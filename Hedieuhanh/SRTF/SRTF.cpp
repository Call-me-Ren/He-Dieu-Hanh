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

// Comparator cho priority_queue: chọn Process* có remaining nhỏ hơn.
// Nếu remaining bằng nhau, chọn arrival nhỏ hơn, nếu vẫn bằng, chọn pid nhỏ hơn.
struct CompareProc {
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

    // Sắp theo arrival; nếu bằng, theo pid
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
        });

    priority_queue<Process*, vector<Process*>, CompareProc> pq;
    vector<pair<int, int>> gantt;
    // pair.first = pid (hoặc -1 nếu idle), pair.second = duration liên tục

    int idx = 0;             // chỉ số theo dõi tiến trình tiếp theo chưa cho vào hàng đợi
    int completed = 0;       // đếm số tiến trình đã hoàn thành
    int currentTime = 0;     // thời gian mô phỏng

    // Nếu tiến trình đầu tiên không đến ở thời 0, CPU phải idle trước
    if (procs[0].arrival > 0) {
        gantt.push_back({ -1, procs[0].arrival });
        currentTime = procs[0].arrival;
    }

    // Bắt đầu đổ tiến trình đã tới vào hàng đợi
    while (idx < n && procs[idx].arrival <= currentTime) {
        pq.push(&procs[idx]);
        idx++;
    }

    int lastPid = -2;  // để kiểm tra nếu segment Gantt thay đổi PID
    int segLen = 0;    // độ dài segment hiện tại

    // Vòng lặp chính: chạy cho đến khi tất cả tiến trình hoàn thành
    while (completed < n) {
        if (pq.empty()) {
            // Nếu queue trống nhưng vẫn còn tiến trình chưa đến, CPU idle đến khi arrival
            int nextArrival = procs[idx].arrival;
            if (lastPid == -1) {
                // tiếp tục nối vào segment Idle
                gantt.back().second += (nextArrival - currentTime);
            }
            else {
                // tạo segment Idle mới
                gantt.push_back({ -1, nextArrival - currentTime });
            }
            currentTime = nextArrival;
            // Đổ thêm tiến trình đã đến
            while (idx < n && procs[idx].arrival <= currentTime) {
                pq.push(&procs[idx]);
                idx++;
            }
            lastPid = -1;
            continue;
        }

        // Lấy tiến trình có remaining nhỏ nhất
        Process* cur = pq.top();
        pq.pop();

        // Nếu đây là lần đầu Process này chạy, gán start time
        if (cur->start == -1) {
            cur->start = currentTime;
        }

        // Thực thi 1 đơn vị thời gian
        int runPid = cur->pid;
        if (lastPid == runPid) {
            // nếu cùng pid với segment trước, kéo dài segment
            gantt.back().second += 1;
        }
        else {
            // tạo segment mới (có thể là thay đổi từ Idle sang process hoặc process này khác process kia)
            gantt.push_back({ runPid, 1 });
        }
        lastPid = runPid;
        cur->remaining -= 1;
        currentTime += 1;

        // Sau khi chạy 1 đơn vị, đổ thêm các tiến trình mới đến ở thời currentTime
        while (idx < n && procs[idx].arrival <= currentTime) {
            pq.push(&procs[idx]);
            idx++;
        }

        // Nếu cur chưa hoàn thành, push lại vào queue
        if (cur->remaining > 0) {
            pq.push(cur);
        }
        else {
            // Nếu cur vừa hoàn thành, ghi finish/time và tăng completed
            cur->finish = currentTime;
            cur->turnaround = cur->finish - cur->arrival;
            cur->waiting = cur->turnaround - cur->burst;
            completed++;
        }
    }

    // In Gantt Chart ra
    cout << "Gantt Chart (SRTF):\n|";
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
