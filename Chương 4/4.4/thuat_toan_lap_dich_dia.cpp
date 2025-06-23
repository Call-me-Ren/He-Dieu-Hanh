#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;

void printResult(const vector<int>& sequence, int startHead) {
    int total = 0;
    int head = startHead;
    cout << "Head moves: " << head;
    for (int cyl : sequence) {
        cout << " -> " << cyl;
    }
    cout << "\n";

    cout << "Step distances: ";
    for (int i = 0; i < sequence.size(); ++i) {
        int dist = abs(sequence[i] - head);
        total += dist;
        cout << dist;
        if (i < sequence.size() - 1) cout << " + ";
        head = sequence[i];
    }
    cout << " = " << total << " cylinders\n";
}

// FIFO
void fifo(const vector<int>& requests, int head) {
    cout << "\n--- FIFO ---\n";
    printResult(requests, head);
}

// SSTF
void sstf(vector<int> requests, int head) {
    cout << "\n--- SSTF ---\n";
    vector<int> sequence;
    int startHead = head;
    while (!requests.empty()) {
        auto it = min_element(requests.begin(), requests.end(), [head](int a, int b) {
            return abs(a - head) < abs(b - head);
            });
        sequence.push_back(*it);
        head = *it;
        requests.erase(it);
    }
    printResult(sequence, startHead);
}

// SCAN (fixed logic for both directions)
void scan(const vector<int>& requests, int head, int direction, int minCyl, int maxCyl) {
    cout << "\n--- SCAN ---\n";
    vector<int> left, right, sequence;
    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    int startHead = head;
    if (direction == -1) {
        reverse(left.begin(), left.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
        if (!left.empty()) sequence.push_back(minCyl);
        sequence.insert(sequence.end(), right.begin(), right.end());
    }
    else {
        sequence.insert(sequence.end(), right.begin(), right.end());
        if (!right.empty()) sequence.push_back(maxCyl);
        reverse(left.begin(), left.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
    }
    printResult(sequence, startHead);
}

// LOOK
void look(const vector<int>& requests, int head, int direction) {
    cout << "\n--- LOOK ---\n";
    vector<int> left, right, sequence;
    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    if (direction == -1) {
        reverse(left.begin(), left.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
        sequence.insert(sequence.end(), right.begin(), right.end());
    }
    else {
        sequence.insert(sequence.end(), right.begin(), right.end());
        reverse(left.begin(), left.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
    }
    printResult(sequence, head);
}

// C-SCAN
void cscan(const vector<int>& requests, int head, int direction, int minCyl, int maxCyl) {
    cout << "\n--- C-SCAN ---\n";
    vector<int> left, right, sequence;
    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    if (direction == -1) {
        reverse(left.begin(), left.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
        sequence.push_back(minCyl);
        sequence.push_back(maxCyl);
        reverse(right.begin(), right.end());
        sequence.insert(sequence.end(), right.begin(), right.end());
    }
    else {
        sequence.insert(sequence.end(), right.begin(), right.end());
        sequence.push_back(maxCyl);
        sequence.push_back(minCyl);
        sequence.insert(sequence.end(), left.begin(), left.end());
    }
    printResult(sequence, head);
}

// C-LOOK
void clook(const vector<int>& requests, int head, int direction) {
    cout << "\n--- C-LOOK ---\n";
    vector<int> left, right, sequence;
    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    if (direction == -1) {
        reverse(left.begin(), left.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
        reverse(right.begin(), right.end());
        sequence.insert(sequence.end(), right.begin(), right.end());
    }
    else {
        sequence.insert(sequence.end(), right.begin(), right.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
    }
    printResult(sequence, head);
}

int main() {
    int minCyl, maxCyl, currentHead, previousHead;
    vector<int> requests;
    cout << "Enter min cylinder: "; cin >> minCyl;
    cout << "Enter max cylinder: "; cin >> maxCyl;
    cout << "Enter current head position: "; cin >> currentHead;
    string choicePrev;
    cout << "Do you want to enter previous head position? (yes/no): "; cin >> choicePrev;
    int direction = 1;
    if (choicePrev == "yes" || choicePrev == "Yes" || choicePrev == "y") {
        cout << "Enter previous head position: "; cin >> previousHead;
        direction = (currentHead - previousHead >= 0) ? 1 : -1;
    }
    int n;
    cout << "Enter number of requests: "; cin >> n;
    cout << "Enter cylinder requests:\n";
    for (int i = 0; i < n; ++i) { int x; cin >> x; requests.push_back(x); }
    vector<int> sorted = requests;
    sorted.push_back(minCyl); sorted.push_back(maxCyl);
    sort(sorted.begin(), sorted.end());
    cout << "\nSorted cylinders (with boundaries): ";
    for (int x : sorted) cout << x << " "; cout << "\n";
    cout << "\nChoose algorithm:\n";
    cout << "1. FIFO\n2. SSTF\n3. SCAN\n4. LOOK\n5. C-SCAN\n6. C-LOOK\n";
    int choice; cin >> choice;
    if (choice >= 3 && choice <= 6) {
        cout << "Choose direction (1 = Right, -1 = Left): "; cin >> direction;
    }
    switch (choice) {
    case 1: fifo(requests, currentHead); break;
    case 2: sstf(requests, currentHead); break;
    case 3: scan(requests, currentHead, direction, minCyl, maxCyl); break;
    case 4: look(requests, currentHead, direction); break;
    case 5: cscan(requests, currentHead, direction, minCyl, maxCyl); break;
    case 6: clook(requests, currentHead, direction); break;
    default: cout << "Invalid choice.\n";
    }
    return 0;
}
