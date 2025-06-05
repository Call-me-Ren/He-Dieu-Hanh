#include <iostream>
#include <vector>
#include <algorithm>
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

    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
        });

    int currentTime = 0;
    vector<pair<int, int>> gantt; // (pid hoặc -1 để đánh dấu idle, duration)

    for (auto& p : procs) {
        if (currentTime < p.arrival) {
            gantt.push_back({ -1, p.arrival - currentTime });
            currentTime = p.arrival;
        }
        p.start = currentTime;
        p.finish = p.start + p.burst;
        p.turnaround = p.finish - p.arrival;
        p.waiting = p.turnaround - p.burst;
        gantt.push_back({ p.pid, p.burst });
        currentTime = p.finish;
    }

    // In Gantt Chart
    cout << "Gantt Chart (FCFS):\n|";
    currentTime = 0;
    for (auto& seg : gantt) {
        if (seg.first == -1) {
            cout << " Idle(" << seg.second << ") |";
        }
        else {
            cout << " P" << seg.first << "(" << seg.second << ") |";
        }
        currentTime += seg.second;
    }
    cout << "\n0";
    currentTime = 0;
    for (auto& seg : gantt) {
        currentTime += seg.second;
        cout << "\t" << currentTime;
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
