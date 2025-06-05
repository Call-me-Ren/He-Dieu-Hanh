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
    int start;
    int finish;
    int turnaround;
    int waiting;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Process> procs(n);
    for (int i = 0; i < n; i++) {
        cin >> procs[i].pid >> procs[i].arrival >> procs[i].burst;
        procs[i].start = -1;
        procs[i].finish = -1;
    }

    // Sắp theo arrival; nếu bằng nhau, theo pid
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
        });

    int currentTime = 0;
    int idx = 0;
    vector<Process> doneList;

    // Comparator cho priority_queue: chọn burst nhỏ nhất; nếu bằng, arrival nhỏ nhất; nếu vẫn bằng, pid nhỏ nhất
    auto cmp = [](const Process& a, const Process& b) {
        if (a.burst != b.burst) return a.burst > b.burst;
        if (a.arrival != b.arrival) return a.arrival > b.arrival;
        return a.pid > b.pid;
        };
    priority_queue<Process, vector<Process>, decltype(cmp)> pq(cmp);

    // Bắt đầu tại arrival đầu tiên
    if (idx < n) currentTime = procs[idx].arrival;

    while (idx < n || !pq.empty()) {
        // Đổ các tiến trình đã đến vào priority_queue
        while (idx < n && procs[idx].arrival <= currentTime) {
            pq.push(procs[idx]);
            idx++;
        }
        if (pq.empty()) {
            // Nếu chưa có tiến trình trong PQ, nhảy tới arrival tiếp theo
            currentTime = procs[idx].arrival;
            continue;
        }
        // Lấy tiến trình có burst nhỏ nhất
        Process p = pq.top();
        pq.pop();
        p.start = currentTime;
        p.finish = p.start + p.burst;
        p.turnaround = p.finish - p.arrival;
        p.waiting = p.turnaround - p.burst;
        currentTime = p.finish;
        doneList.push_back(p);
    }

    // In Gantt Chart
    cout << "Gantt Chart (SJF Non-Preemptive):\n|";
    currentTime = 0;
    for (auto& p : doneList) {
        if (currentTime < p.start) {
            cout << " Idle(" << (p.start - currentTime) << ") |";
            currentTime = p.start;
        }
        cout << " P" << p.pid << "(" << p.burst << ") |";
        currentTime = p.finish;
    }
    cout << "\n0";
    currentTime = 0;
    for (auto& p : doneList) {
        if (currentTime < p.start) {
            currentTime = p.start;
            cout << "\t" << currentTime;
        }
        currentTime += p.burst;
        cout << "\t" << currentTime;
    }
    cout << "\n\n";

    // In bảng kết quả chi tiết
    cout << "PID\tArr\tBurst\tStart\tFinish\tTAT\tWT\n";
    double sumTAT = 0, sumWT = 0;
    for (auto& p : doneList) {
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
