#include <iostream>
#include <vector>
using namespace std;

int main() {
    int P, R;
    cin >> P >> R;

    vector<vector<int>> max(P, vector<int>(R));
    vector<vector<int>> allocation(P, vector<int>(R));
    vector<vector<int>> need(P, vector<int>(R));
    vector<int> available(R);

    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            cin >> max[i][j];

    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            cin >> allocation[i][j];

    for (int i = 0; i < R; i++)
        cin >> available[i];

    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];

    vector<bool> finish(P, false);
    vector<int> safeSeq;
    vector<int> work = available;

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                bool canRun = true;
                for (int j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        canRun = false;
                        break;
                    }
                }

                if (canRun) {
                    for (int j = 0; j < R; j++)
                        work[j] += allocation[i][j];
                    safeSeq.push_back(i);
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }

        if (!found) {
            cout << "System is NOT in a safe state.\n";
            return 0;
        }
    }

    cout << "System is in a SAFE state.\n";
    cout << "Safe sequence: ";
    for (int i = 0; i < P; i++)
        cout << "P" << safeSeq[i] << (i == P - 1 ? "\n" : " -> ");

    return 0;
}
