#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

void input(int& frameSize, vector<int>& pages) {
    cout << "Enter number of frames: ";
    cin >> frameSize;

    cout << "Enter page reference string (end with -1): ";
    int x;
    while (cin >> x && x != -1) {
        pages.push_back(x);
    }
}

// FIFO
void fifo(int frameSize, const vector<int>& pages) {
    cout << "\n--- FIFO ---\n";
    vector<int> frames;
    int faults = 0;

    for (int page : pages) {
        auto it = find(frames.begin(), frames.end(), page);
        if (it == frames.end()) {
            if (frames.size() == frameSize)
                frames.erase(frames.begin());
            frames.push_back(page);
            faults++;
            cout << "Page " << page << " -> Fault\n";
        }
        else {
            cout << "Page " << page << " -> Hit\n";
        }
    }
    cout << "Total Page Faults: " << faults << endl;
}

// LRU
void lru(int frameSize, const vector<int>& pages) {
    cout << "\n--- LRU ---\n";
    vector<int> frames;
    unordered_map<int, int> lastUsed;
    int faults = 0;

    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it == frames.end()) {
            if (frames.size() == frameSize) {
                // Find least recently used
                int lruPage = frames[0];
                for (int f : frames) {
                    if (lastUsed[f] < lastUsed[lruPage])
                        lruPage = f;
                }
                frames.erase(find(frames.begin(), frames.end(), lruPage));
            }
            frames.push_back(page);
            faults++;
            cout << "Page " << page << " -> Fault\n";
        }
        else {
            cout << "Page " << page << " -> Hit\n";
        }
        lastUsed[page] = i;
    }
    cout << "Total Page Faults: " << faults << endl;
}

// OPT (Optimal)
void optimal(int frameSize, const vector<int>& pages) {
    cout << "\n--- Optimal ---\n";
    vector<int> frames;
    int faults = 0;

    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it == frames.end()) {
            if (frames.size() == frameSize) {
                int farthest = -1, idx = -1;
                for (int f = 0; f < frames.size(); ++f) {
                    int nextUse = -1;
                    for (int j = i + 1; j < pages.size(); ++j) {
                        if (pages[j] == frames[f]) {
                            nextUse = j;
                            break;
                        }
                    }
                    if (nextUse == -1) {
                        idx = f;
                        break;
                    }
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        idx = f;
                    }
                }
                frames.erase(frames.begin() + idx);
            }
            frames.push_back(page);
            faults++;
            cout << "Page " << page << " -> Fault\n";
        }
        else {
            cout << "Page " << page << " -> Hit\n";
        }
    }
    cout << "Total Page Faults: " << faults << endl;
}

int main() {
    int frameSize;
    vector<int> pages;

    input(frameSize, pages);

    cout << "\nChoose Replacement Algorithm:\n";
    cout << "1. FIFO\n2. LRU\n3. OPT\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1: fifo(frameSize, pages); break;
    case 2: lru(frameSize, pages); break;
    case 3: optimal(frameSize, pages); break;
    default: cout << "Invalid choice.\n";
    }

    return 0;
}
