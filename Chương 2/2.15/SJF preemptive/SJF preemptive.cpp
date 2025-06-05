#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int start;
    int finish;
    int turnaround;
    int waiting;
};

// Comparator cho priority_queue: chọn Process* có remaining nhỏ;
// nếu remaining bằng nhau thì chọn arrival nhỏ; nếu vẫn bằng thì pid nhỏ.
struct CompareRemaining {
    bool operator()(const Process* a, const Process* b) const {
        if (a->remaining != b->remaining)
            return a->remaining > b->remaining;
        if (a->arrival != b->arrival)
            return a->arrival > b->arrival;
        return a->pid > b->pid;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Process> procs(n);
    for (int i = 0; i < n; i++) {
        cin >> procs[i].pid >> procs[i].arrival >> procs[i].burst;
        procs[i].remaining = procs[i].burst;
        procs[i].start = -1;
        procs[i].finish = -1;
        procs[i].turnaround = 0;
        procs[i].waiting = 0;
    }

    // Sắp theo arrival; nếu bằng nhau, theo pid
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
        });

    priority_queue<Process*, vector<Process*>, CompareRemaining> pq;
    vector<pair<int, int>> gantt;
    // pair.first = pid (hoặc -1 nếu idle), pair.second = duration liên tục

    int idx = 0;             // chỉ số process tiếp theo chưa đưa vào queue
    int completed = 0;       // đếm đã hoàn thành
    int currentTime = 0;     // thời gian mô phỏng

    // Nếu tiến trình đầu tiên không đến ở thời 0, CPU phải idle trước
    if (procs[0].arrival > 0) {
        gantt.push_back({ -1, procs[0].arrival });
        currentTime = procs[0].arrival;
    }

    // Đổ initial processes có arrival <= currentTime vào queue
    while (idx < n && procs[idx].arrival <= currentTime) {
        pq.push(&procs[idx]);
        idx++;
    }

    int lastPid = -2;  // để kiểm tra việc nối segment Gantt
    // Vòng lặp cho đến khi tất cả hoàn thành
    while (completed < n) {
        if (pq.empty()) {
            // Queue trống nhưng vẫn còn process chưa đến
            int nextArrival = procs[idx].arrival;
            if (lastPid == -1) {
                // Nối tiếp segment Idle
                gantt.back().second += (nextArrival - currentTime);
            }
            else {
                // Tạo segment Idle mới
                gantt.push_back({ -1, nextArrival - currentTime });
            }
            currentTime = nextArrival;
            // Đổ thêm process mới đến bằng thời điểm mới
            while (idx < n && procs[idx].arrival <= currentTime) {
                pq.push(&procs[idx]);
                idx++;
            }
            lastPid = -1;
            continue;
        }

        // Chọn process có remaining nhỏ nhất
        Process* cur = pq.top();
        pq.pop();

        // Nếu lần đầu process này chạy, ghi start time
        if (cur->start == -1) {
            cur->start = currentTime;
        }

        // Chạy 1 đơn vị thời gian
        int runPid = cur->pid;
        if (lastPid == runPid) {
            // Nối tiếp segment với cùng PID
            gantt.back().second += 1;
        }
        else {
            // Tạo segment mới
            gantt.push_back({ runPid, 1 });
        }
        lastPid = runPid;

        cur->remaining -= 1;
        currentTime += 1;

        // Sau khi chạy 1 đơn vị, đổ thêm process mới đến trong thời gian này
        while (idx < n && procs[idx].arrival <= currentTime) {
            pq.push(&procs[idx]);
            idx++;
        }

        if (cur->remaining > 0) {
            // Chưa xong, push lại
            pq.push(cur);
        }
        else {
            // Hoàn thành, ghi finish, tính TAT và WT
            cur->finish = currentTime;
            cur->turnaround = cur->finish - cur->arrival;
            cur->waiting = cur->turnaround - cur->burst;
            completed++;
        }
    }

    // In Gantt Chart
    cout << "Gantt Chart (SJF Preemptive):\n|";
    int t = 0;
    for (auto& seg : gantt) {
        if (seg.first == -1) {
            cout << " Idle(" << seg.second << ") |";
        }
        else {
            cout << " P" << seg.first << "(" << seg.second << ") |";
        }
        t += seg.second;
    }
    cout << "\n0";
    t = 0;
    for (auto& seg : gantt) {
        t += seg.second;
        cout << "\t" << t;
    }
    cout << "\n\n";

    // In bảng kết quả chi tiết
    cout << "PID\tArr\tBurst\tStart\tFinish\tTAT\tWT\n";
    double sumTAT = 0, sumWT = 0;
    for (auto& p : procs) {
        cout << p.pid << "\t"
            << p.arrival << "\t"
            << p.burst << "\t"
            << p.start << "\t"
            << p.finish << "\t"
            << p.turnaround << "\t"
            << p.waiting << "\n";
        sumTAT += p.turnaround;
        sumWT += p.waiting;
    }
    cout << fixed << setprecision(2);
    cout << "Average TAT = " << (sumTAT / n) << "\n";
    cout << "Average WT  = " << (sumWT / n) << "\n";

    return 0;
}
