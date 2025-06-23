#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

void printResult(const vector<int>& sequence, int head) {
    int total = 0;
    cout << "Head moves: ";
    for (int i = 0; i < sequence.size(); ++i) {
        cout << sequence[i];
        if (i < sequence.size() - 1)
            cout << " -> ";
    }
    cout << "\n";

    cout << "Step distances: ";
    for (int i = 0; i < sequence.size(); ++i) {
        int dist = abs(head - sequence[i]);
        total += dist;
        cout << dist;
        if (i < sequence.size() - 1) cout << " + ";
        head = sequence[i];
    }
    cout << " = " << total << endl;
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
    while (!requests.empty()) {
        auto it = min_element(requests.begin(), requests.end(), [head](int a, int b) {
            return abs(a - head) < abs(b - head);
            });
        sequence.push_back(*it);
        head = *it;
        requests.erase(it);
    }
    printResult(sequence, head);
}

// SCAN
void scan(vector<int> requests, int head, int direction, int minCyl, int maxCyl) {
    cout << "\n--- SCAN ---\n";
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
        sequence.insert(sequence.end(), right.begin(), right.end());
    }
    else {
        sequence.insert(sequence.end(), right.begin(), right.end());
        sequence.push_back(maxCyl);
        reverse(left.begin(), left.end());
        sequence.insert(sequence.end(), left.begin(), left.end());
    }

    printResult(sequence, head);
}

// LOOK
void look(vector<int> requests, int head, int direction) {
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
void cscan(vector<int> requests, int head, int minCyl, int maxCyl) {
    cout << "\n--- C-SCAN ---\n";
    vector<int> left, right, sequence;

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    sequence.insert(sequence.end(), right.begin(), right.end());
    sequence.push_back(maxCyl);
    sequence.push_back(minCyl);
    sequence.insert(sequence.end(), left.begin(), left.end());

    printResult(sequence, head);
}

// C-LOOK
void clook(vector<int> requests, int head) {
    cout << "\n--- C-LOOK ---\n";
    vector<int> left, right, sequence;

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    sequence.insert(sequence.end(), right.begin(), right.end());
    sequence.insert(sequence.end(), left.begin(), left.end());

    printResult(sequence, head);
}

int main() {
    int minCyl, maxCyl, currentHead, previousHead;
    vector<int> requests;

    cout << "Enter min cylinder: ";
    cin >> minCyl;
    cout << "Enter max cylinder: ";
    cin >> maxCyl;

    cout << "Enter current head position: ";
    cin >> currentHead;

    cout << "Enter previous head position: ";
    cin >> previousHead;
    requests.push_back(previousHead); // ✅ Thêm previousHead vào dãy yêu cầu

    int n;
    cout << "Enter number of requests: ";
    cin >> n;

    cout << "Enter cylinder requests:\n";
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        requests.push_back(x);
    }

    vector<int> sorted = requests;
    sorted.push_back(minCyl);
    sorted.push_back(maxCyl);
    sort(sorted.begin(), sorted.end());
    cout << "\nSorted cylinders (with boundaries): ";
    for (int x : sorted) cout << x << " ";
    cout << "\n";

    cout << "\nChoose algorithm:\n";
    cout << "1. FIFO\n2. SSTF\n3. SCAN\n4. LOOK\n5. C-SCAN\n6. C-LOOK\n";
    int choice;
    cin >> choice;

    int direction = (currentHead - previousHead >= 0) ? 1 : -1;

    switch (choice) {
    case 1: fifo(requests, currentHead); break;
    case 2: sstf(requests, currentHead); break;
    case 3: scan(requests, currentHead, direction, minCyl, maxCyl); break;
    case 4: look(requests, currentHead, direction); break;
    case 5: cscan(requests, currentHead, minCyl, maxCyl); break;
    case 6: clook(requests, currentHead); break;
    default: cout << "Invalid choice.\n";
    }

    return 0;
}
