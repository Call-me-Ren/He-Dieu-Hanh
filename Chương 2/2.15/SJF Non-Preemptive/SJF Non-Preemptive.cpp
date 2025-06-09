#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid, arrival, burst, start, finish, turnaround, waiting;
};

int main() {
    int n;
    cin >> n;  // Không in lời nhắc

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i].pid >> p[i].arrival >> p[i].burst;  // Nhập liền mạch
    }

    vector<bool> done(n, false);
    int completed = 0, currentTime = 0;
    double totalTAT = 0, totalWT = 0;

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= currentTime && p[i].burst < minBurst) {
                minBurst = p[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        p[idx].start = currentTime;
        p[idx].finish = currentTime + p[idx].burst;
        p[idx].turnaround = p[idx].finish - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        done[idx] = true;
        completed++;
        currentTime = p[idx].finish;

        totalTAT += p[idx].turnaround;
        totalWT += p[idx].waiting;
    }

    cout << fixed << setprecision(2);
    cout << "\nPID\tArrival\tBurst\tStart\tFinish\tTAT\tWT\n";
    for (auto& proc : p) {
        cout << proc.pid << "\t" << proc.arrival << "\t" << proc.burst << "\t"
            << proc.start << "\t" << proc.finish << "\t"
            << proc.turnaround << "\t" << proc.waiting << "\n";
    }

    cout << "Average TAT: " << totalTAT / n << "\n";
    cout << "Average WT: " << totalWT / n << "\n";

    return 0;
}
