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
    int remaining;
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
        procs[i].remaining = procs[i].burst;
    }
    int quantum;
    cin >> quantum;

    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
        });

    queue<int> q;
    vector<int> inQueue(n, 0);
    vector<pair<int, int>> gantt;
    int idx = 0;
    int currentTime = 0;

    if (idx < n) currentTime = procs[idx].arrival;
    while (idx < n && procs[idx].arrival <= currentTime) {
        q.push(idx);
        inQueue[idx] = 1;
        idx++;
    }

    while (!q.empty()) {
        int id = q.front();
        q.pop();
        Process& p = procs[id];

        if (p.start == -1) p.start = currentTime;
        int used = min(quantum, p.remaining);
        p.remaining -= used;
        gantt.push_back({ p.pid, used });
        currentTime += used;

        while (idx < n && procs[idx].arrival <= currentTime) {
            if (!inQueue[idx]) {
                q.push(idx);
                inQueue[idx] = 1;
            }
            idx++;
        }

        if (p.remaining > 0) {
            q.push(id);
        }
        else {
            p.finish = currentTime;
            p.turnaround = p.finish - p.arrival;
            p.waiting = p.turnaround - p.burst;
        }

        if (q.empty() && idx < n) {
            if (currentTime < procs[idx].arrival) {
                gantt.push_back({ -1, procs[idx].arrival - currentTime });
                currentTime = procs[idx].arrival;
            }
            while (idx < n && procs[idx].arrival <= currentTime) {
                if (!inQueue[idx]) {
                    q.push(idx);
                    inQueue[idx] = 1;
                }
                idx++;
            }
        }
    }

    cout << "Gantt Chart (RR, Q=" << quantum << "):\n|";
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
