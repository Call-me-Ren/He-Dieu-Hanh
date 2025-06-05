#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Hàm hiển thị kết quả cấp phát
void displayAllocation(const vector<int>& allocation) {
    for (int i = 0; i < allocation.size(); i++) {
        cout << "Tien trinh " << i + 1 << " -> ";
        if (allocation[i] != -1)
            cout << "Vung nho " << allocation[i] + 1 << endl;
        else
            cout << "Khong duoc cap phat\n";
    }
}

// First Fit
void firstFit(vector<int> blocks, vector<int> processes) {
    vector<int> allocation(processes.size(), -1);

    for (int i = 0; i < processes.size(); i++) {
        for (int j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                allocation[i] = j;
                blocks[j] -= processes[i];
                break;
            }
        }
    }

    cout << "\n=== First Fit ===\n";
    displayAllocation(allocation);
}

// Best Fit
void bestFit(vector<int> blocks, vector<int> processes) {
    vector<int> allocation(processes.size(), -1);

    for (int i = 0; i < processes.size(); i++) {
        int bestIdx = -1;
        for (int j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                if (bestIdx == -1 || blocks[j] < blocks[bestIdx])
                    bestIdx = j;
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blocks[bestIdx] -= processes[i];
        }
    }

    cout << "\n=== Best Fit ===\n";
    displayAllocation(allocation);
}

// Worst Fit
void worstFit(vector<int> blocks, vector<int> processes) {
    vector<int> allocation(processes.size(), -1);

    for (int i = 0; i < processes.size(); i++) {
        int worstIdx = -1;
        for (int j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                if (worstIdx == -1 || blocks[j] > blocks[worstIdx])
                    worstIdx = j;
            }
        }

        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blocks[worstIdx] -= processes[i];
        }
    }

    cout << "\n=== Worst Fit ===\n";
    displayAllocation(allocation);
}

int main() {
    int nBlocks, nProcesses;

    cout << "Nhap so vung nho (block): ";
    cin >> nBlocks;
    vector<int> blocks(nBlocks);
    cout << "Nhap kich thuoc cac vung nho:\n";
    for (int i = 0; i < nBlocks; i++)
        cin >> blocks[i];

    cout << "Nhap so tien trinh: ";
    cin >> nProcesses;
    vector<int> processes(nProcesses);
    cout << "Nhap kich thuoc cac tien trinh:\n";
    for (int i = 0; i < nProcesses; i++)
        cin >> processes[i];

    // Gọi các thuật toán
    firstFit(blocks, processes);
    bestFit(blocks, processes);
    worstFit(blocks, processes);

    return 0;
}
