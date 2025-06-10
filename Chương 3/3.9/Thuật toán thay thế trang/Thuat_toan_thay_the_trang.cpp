#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <string>
using namespace std;

// Input function for page reference string
void inputPages(vector<int>& pages) {
    string line;
    cout << "Enter the page reference string: ";
    getline(cin, line);
    int x;
    size_t i = 0;
    while (i < line.length()) {
        if (isdigit(line[i])) {
            x = 0;
            while (i < line.length() && isdigit(line[i])) {
                x = x * 10 + (line[i] - '0');
                i++;
            }
            pages.push_back(x);
        }
        else i++;
    }
}

// First-In-First-Out (FIFO) algorithm
int fifo(const vector<int>& pages, int frames) {
    unordered_set<int> memory;
    queue<int> order;
    int faults = 0;

    for (int page : pages) {
        if (memory.find(page) == memory.end()) {
            if (memory.size() == frames) {
                int old = order.front(); order.pop();
                memory.erase(old);
            }
            memory.insert(page);
            order.push(page);
            faults++;
        }
    }
    return faults;
}

// Least Recently Used (LRU) algorithm
int lru(const vector<int>& pages, int frames) {
    unordered_set<int> memory;
    unordered_map<int, int> lastUsed;
    int faults = 0;

    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        if (memory.find(page) == memory.end()) {
            if (memory.size() == frames) {
                int lru_page = page;
                int min_time = i;
                for (int p : memory) {
                    if (lastUsed[p] < min_time) {
                        min_time = lastUsed[p];
                        lru_page = p;
                    }
                }
                memory.erase(lru_page);
            }
            memory.insert(page);
            faults++;
        }
        lastUsed[page] = i;
    }
    return faults;
}

// Optimal (OPT) algorithm
int opt(const vector<int>& pages, int frames) {
    unordered_set<int> memory;
    int faults = 0;

    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        if (memory.find(page) == memory.end()) {
            if (memory.size() == frames) {
                int farthest = i, victim = -1;
                for (int p : memory) {
                    int j;
                    for (j = i + 1; j < pages.size(); ++j) {
                        if (pages[j] == p) break;
                    }
                    if (j > farthest) {
                        farthest = j;
                        victim = p;
                    }
                }
                memory.erase(victim);
            }
            memory.insert(page);
            faults++;
        }
    }
    return faults;
}

int main() {
    vector<int> pages;
    int frames, choice;

    inputPages(pages);
    cout << "Enter number of page frames: ";
    cin >> frames;
    cout << "Choose algorithm (1-FIFO, 2-LRU, 3-OPT): ";
    cin >> choice;

    int faults = 0;
    switch (choice) {
    case 1: faults = fifo(pages, frames); break;
    case 2: faults = lru(pages, frames); break;
    case 3: faults = opt(pages, frames); break;
    default: cout << "Invalid choice.\n"; return 1;
    }

    cout << "Total page faults: " << faults << endl;
    return 0;
}
