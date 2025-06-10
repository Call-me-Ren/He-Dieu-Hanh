#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

struct Block {
    int size;
    int originalSize;
    bool allocated;
};

struct Process {
    int size;
};

void splitInput(const string& line, vector<int>& values) {
    stringstream ss(line);
    int x;
    while (ss >> x) {
        values.push_back(x);
    }
}

void input(vector<Block>& blocks, vector<Process>& processes) {
    string line;
    vector<int> sizes;

    cout << "Enter memory block sizes: ";
    getline(cin, line);
    splitInput(line, sizes);
    for (int sz : sizes) {
        blocks.push_back({ sz, sz, false });
    }

    sizes.clear();
    cout << "Enter process sizes: ";
    getline(cin, line);
    splitInput(line, sizes);
    for (int sz : sizes) {
        processes.push_back({ sz });
    }
}

// First Fit
void firstFit(vector<Block> blocks, const vector<Process>& processes) {
    cout << "\n--- First Fit ---\n";
    for (size_t i = 0; i < processes.size(); ++i) {
        bool allocated = false;
        for (size_t j = 0; j < blocks.size(); ++j) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                cout << "Process " << i + 1 << " (" << processes[i].size << "KB)"
                    << " -> Block " << j + 1 << " (" << blocks[j].originalSize << "KB), Remaining: "
                    << blocks[j].size - processes[i].size << "KB\n";
                blocks[j].size -= processes[i].size;
                blocks[j].allocated = true;
                allocated = true;
                break;
            }
        }
        if (!allocated)
            cout << "Process " << i + 1 << " (" << processes[i].size << "KB) -> Not Allocated\n";
    }
}

// Best Fit
void bestFit(vector<Block> blocks, const vector<Process>& processes) {
    cout << "\n--- Best Fit ---\n";
    for (size_t i = 0; i < processes.size(); ++i) {
        int bestIdx = -1;
        for (size_t j = 0; j < blocks.size(); ++j) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                if (bestIdx == -1 || blocks[j].size < blocks[bestIdx].size)
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            cout << "Process " << i + 1 << " (" << processes[i].size << "KB)"
                << " -> Block " << bestIdx + 1 << " (" << blocks[bestIdx].originalSize << "KB), Remaining: "
                << blocks[bestIdx].size - processes[i].size << "KB\n";
            blocks[bestIdx].size -= processes[i].size;
            blocks[bestIdx].allocated = true;
        }
        else {
            cout << "Process " << i + 1 << " (" << processes[i].size << "KB) -> Not Allocated\n";
        }
    }
}

// Worst Fit
void worstFit(vector<Block> blocks, const vector<Process>& processes) {
    cout << "\n--- Worst Fit ---\n";
    for (size_t i = 0; i < processes.size(); ++i) {
        int worstIdx = -1;
        for (size_t j = 0; j < blocks.size(); ++j) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                if (worstIdx == -1 || blocks[j].size > blocks[worstIdx].size)
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            cout << "Process " << i + 1 << " (" << processes[i].size << "KB)"
                << " -> Block " << worstIdx + 1 << " (" << blocks[worstIdx].originalSize << "KB), Remaining: "
                << blocks[worstIdx].size - processes[i].size << "KB\n";
            blocks[worstIdx].size -= processes[i].size;
            blocks[worstIdx].allocated = true;
        }
        else {
            cout << "Process " << i + 1 << " (" << processes[i].size << "KB) -> Not Allocated\n";
        }
    }
}

// Last Fit
void lastFit(vector<Block> blocks, const vector<Process>& processes) {
    cout << "\n--- Last Fit ---\n";
    for (size_t i = 0; i < processes.size(); ++i) {
        bool allocated = false;
        for (int j = blocks.size() - 1; j >= 0; --j) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                cout << "Process " << i + 1 << " (" << processes[i].size << "KB)"
                    << " -> Block " << j + 1 << " (" << blocks[j].originalSize << "KB), Remaining: "
                    << blocks[j].size - processes[i].size << "KB\n";
                blocks[j].size -= processes[i].size;
                blocks[j].allocated = true;
                allocated = true;
                break;
            }
        }
        if (!allocated)
            cout << "Process " << i + 1 << " (" << processes[i].size << "KB) -> Not Allocated\n";
    }
}

int main() {
    vector<Block> blocks;
    vector<Process> processes;

    input(blocks, processes);

    cout << "\nChoose Allocation Strategy:\n";
    cout << "1. First Fit\n2. Best Fit\n3. Worst Fit\n4. Last Fit\n";
    cout << "Enter your choice (1-4): ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1: firstFit(blocks, processes); break;
    case 2: bestFit(blocks, processes); break;
    case 3: worstFit(blocks, processes); break;
    case 4: lastFit(blocks, processes); break;
    default: cout << "Invalid choice.\n";
    }

    return 0;
}
