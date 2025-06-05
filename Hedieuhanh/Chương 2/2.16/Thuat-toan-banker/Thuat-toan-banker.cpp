#include <iostream>
#include <vector>
using namespace std;

int main() {
    int P, R;
    cout << "Nhap so tien trinh (P): ";
    cin >> P;
    cout << "Nhap so loai tai nguyen (R): ";
    cin >> R;

    vector<vector<int>> max(P, vector<int>(R));
    vector<vector<int>> allocation(P, vector<int>(R));
    vector<vector<int>> need(P, vector<int>(R));
    vector<int> available(R);

    cout << "Nhap ma tran MAX (P x R):\n";
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            cin >> max[i][j];

    cout << "Nhap ma tran ALLOCATION (P x R):\n";
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            cin >> allocation[i][j];

    cout << "Nhap mang AVAILABLE (R phan tu):\n";
    for (int i = 0; i < R; i++)
        cin >> available[i];

    // Tinh NEED = MAX - ALLOCATION
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
            cout << "\nHe thong KHONG an toan!\n";
            return 0;
        }
    }

    cout << "\nHe thong o TRANG THAI AN TOAN.\n";
    cout << "Chuoi an toan (Safe sequence): ";
    for (int i = 0; i < P; i++)
        cout << "P" << safeSeq[i] << (i == P - 1 ? "\n" : " -> ");

    return 0;
}
