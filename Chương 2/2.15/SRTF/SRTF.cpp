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

    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
        });

    priority_queue<Process*, vector<Process*>, CompareProc> pq;
    vector<pair<int, int>> gantt;

    int idx = 0;
    int completed = 0;
    int currentTime = 0;

    if (procs[0].arrival > 0) {
        gantt.push_back({ -1, procs[0].arrival });
        currentTime = procs[0].arrival;
    }

    while (idx < n && procs[idx].arrival <= currentTime) {
        pq.push(&procs[idx]);
        idx++;
    }

    int lastPid = -2;
    int segLen = 0;

    while (completed < n) {
        if (pq.empty()) {
            int nextArrival = procs[idx].arrival;
            if (lastPid == -1) {
                gantt.back().second += (nextArrival - currentTime);
            }
            else {
                gantt.push_back({ -1, nextArrival - currentTime });
            }
            currentTime = nextArrival;
            while (idx < n && procs[idx].arrival <= currentTime) {
                pq.push(&procs[idx]);
                idx++;
            }
            lastPid = -1;
            continue;
        }

        Process* cur = pq.top();
        pq.pop();

        if (cur->start == -1) {
            cur->start = currentTime;
        }

        int runPid = cur->pid;
        if (lastPid == runPid) {
            gantt.back().second += 1;
        }
        else {
            gantt.push_back({ runPid, 1 });
        }
        lastPid = runPid;
        cur->remaining -= 1;
        currentTime += 1;

        while (idx < n && procs[idx].arrival <= currentTime) {
            pq.push(&procs[idx]);
            idx++;
        }

        if (cur->remaining > 0) {
            pq.push(cur);
        }
        else {
            cur->finish = currentTime;
            cur->turnaround = cur->finish - cur->arrival;
            cur->waiting = cur->turnaround - cur->burst;
            completed++;
        }
    }

    cout << fixed << setprecision(2);
    double totalTAT = 0, totalWT = 0;
    cout << "PID\tArrival\tBurst\tStart\tFinish\tTAT\tWT\n";
    for (const auto& p : procs) {
        totalTAT += p.turnaround;
        totalWT += p.waiting;
        cout << p.pid << '\t' << p.arrival << '\t' << p.burst << '\t' << p.start
            << '\t' << p.finish << '\t' << p.turnaround << '\t' << p.waiting << '\n';
    }

    cout << "Avg TAT: " << totalTAT / n << '\n';
    cout << "Avg WT: " << totalWT / n << '\n';

    return 0;
}
