//fcfs
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<int> arrival_time(n), burst_time(n), completion_time(n);
    vector<int> waiting_time(n), tat(n);

    cout << "\nEnter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        cout << "Process[" << i + 1 << "] Arrival Time: ";
        cin >> arrival_time[i];
        cout << "Process[" << i + 1 << "] Burst Time: ";
        cin >> burst_time[i];
    }

    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < arrival_time[i]) {
            current_time = arrival_time[i]; 
        }
        current_time += burst_time[i];
        completion_time[i] = current_time;
    }

    for (int i = 0; i < n; i++) {
        tat[i] = completion_time[i] - arrival_time[i];
        waiting_time[i] = tat[i] - burst_time[i];
    }

    float avg_wait = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        avg_wait += waiting_time[i];
        avg_tat += tat[i];
    }
    avg_wait /= n;
    avg_tat /= n;

    cout << "\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i + 1 << "\t"
             << arrival_time[i] << "\t\t"
             << burst_time[i] << "\t\t"
             << completion_time[i] << "\t\t"
             << tat[i] << "\t\t"
             << waiting_time[i] << endl;
    }
    cout << "\nAverage Waiting Time: " << avg_wait;
    cout << "\nAverage Turnaround Time: " << avg_tat << endl;
    return 0;
}
//sjf pre
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid;
    int at;
    int bt;
    int rt;    // remaining time
    int wt = 0;
    int tat = 0;
    int ct = 0;
};

void srtf(vector<Process>& p) {
    int n = p.size();
    int completed = 0;
    int time = 0;
    int min_rt;
    int shortest = -1;
    bool found = false;

    // Copy burst time into remaining time
    for (int i = 0; i < n; i++)
        p[i].rt = p[i].bt;

    while (completed < n) {
        shortest = -1;
        min_rt = 1e9;
        found = false;

        // Find process with smallest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < min_rt) {
                min_rt = p[i].rt;
                shortest = i;
                found = true;
            }
        }

        if (!found) {
            time++; 
            continue;
        }

        // Execute 1 unit of time
        p[shortest].rt--;
        time++;

        // If process finished
        if (p[shortest].rt == 0) {
            completed++;
            p[shortest].ct = time;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
        }
    }
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        cout << "\nEnter Arrival Time for Process " << i + 1 << ": ";
        cin >> p[i].at;
        cout << "Enter Burst Time for Process " << i + 1 << ": ";
        cin >> p[i].bt;
        p[i].pid = i + 1;
    }

    srtf(p);

    cout << "\nProcess\tAT\tBT\tWT\tTAT\tCT\n";
    double totalWT = 0, totalTAT = 0;

    for (auto& x : p) {
        totalWT += x.wt;
        totalTAT += x.tat;
        cout << "P" << x.pid << "\t" << x.at << "\t" << x.bt
             << "\t" << x.wt << "\t" << x.tat << "\t" << x.ct << "\n";
    }

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << "\n";

    return 0;
}
//roundrobin
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

void roundRobin(vector<string> processes, vector<int> arrival_time, vector<int> burst_time, int quantum) {
    int n = processes.size();
    vector<int> rem_bt = burst_time;     
    vector<int> completion_time(n, 0);
    vector<bool> visited(n, false);      
    queue<int> ready_queue;
    vector<pair<int, string>> gantt;     
    int t = 0;                           

    while (true) {
        for (int i = 0; i < n; i++) {
            if (arrival_time[i] <= t && !visited[i]) {
                ready_queue.push(i);
                visited[i] = true;
            }
        }

        if (ready_queue.empty()) {
            bool all_done = true;
            for (int rt : rem_bt) {
                if (rt > 0) {
                    all_done = false;
                    break;
                }
            }
            if (all_done) break;
            t++;
            continue;
        }
        int i = ready_queue.front();
        ready_queue.pop();

        gantt.push_back({t, processes[i]});  
        if (rem_bt[i] > quantum) {
            t += quantum;
            rem_bt[i] -= quantum;
        } else {
            t += rem_bt[i];
            rem_bt[i] = 0;
            completion_time[i] = t;
        }

        // Add newly arrived processes after execution
        for (int j = 0; j < n; j++) {
            if (arrival_time[j] <= t && !visited[j]) {
                ready_queue.push(j);
                visited[j] = true;
            }
        }

        // If process still not finished, add it back
        if (rem_bt[i] > 0) {
            ready_queue.push(i);
        }

        // Stop when all completed
        bool done = true;
        for (int rt : rem_bt) {
            if (rt > 0) { done = false; break; }
        }
        if (done) break;
    }

    // Calculate TAT and WT
    vector<int> tat(n), wt(n);
    for (int i = 0; i < n; i++) {
        tat[i] = completion_time[i] - arrival_time[i];
        wt[i] = tat[i] - burst_time[i];
    }

    // Print Gantt Chart
    cout << "\nGantt Chart:\n";
    for (auto &entry : gantt) {
        cout << entry.first << " | " << entry.second << " -> ";
    }
    cout << t << endl;

    // Print Table
    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << processes[i] << "\t" << arrival_time[i] << "\t" << burst_time[i]
             << "\t" << completion_time[i] << "\t" << tat[i] << "\t" << wt[i] << endl;
    }
    double avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++) {
        avg_tat += tat[i];
        avg_wt += wt[i];
    }
    avg_tat /= n;
    avg_wt /= n;

    cout << fixed << setprecision(2);
    cout << "\nAverage TAT: " << avg_tat;
    cout << "\nAverage WT: " << avg_wt << endl;
}
int main() {
    vector<string> processes = {"P1", "P2", "P3", "P4"};
    vector<int> arrival_time = {0, 1, 2, 4};
    vector<int> burst_time = {5, 4, 2, 1};
    int quantum = 2;

    roundRobin(processes, arrival_time, burst_time, quantum);
    return 0;
}
//priority 
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Process {
public:
    int pid;        // Process ID
    int at;         // Arrival Time
    int bt;         // Burst Time
    int priority;   // Priority (higher value = higher priority)
    int wt;         // Waiting Time
    int tat;        // Turnaround Time
    int ct;         // Completion Time
    bool finished;  // To track completion
    Process(int id, int arrival, int burst, int pr) {
        pid = id;
        at = arrival;
        bt = burst;
        priority = pr;
        wt = tat = ct = 0;
        finished = false;
    }
};

void priorityScheduling(vector<Process> &processes) {
    int time = 0;
    int completed = 0;
    int n = processes.size();

    while (completed < n) {
        vector<Process*> readyQueue;
        for (auto &p : processes) {
            if (p.at <= time && !p.finished)
                readyQueue.push_back(&p);
        }

        if (!readyQueue.empty()) {
            sort(readyQueue.begin(), readyQueue.end(), [](Process* a, Process* b) {
                if (a->priority == b->priority)
                    return a->at < b->at;
                return a->priority > b->priority; // Higher priority first
            });

            Process* current = readyQueue[0];
            time += current->bt;
            current->ct = time;
            current->tat = current->ct - current->at;
            current->wt = current->tat - current->bt;
            current->finished = true;
            completed++;
        } else {
            time++; // If no process is ready, move time forward
        }
    }
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;

    for (int i = 0; i < n; i++) {
        int at, bt, pr;
        cout << "\nEnter Arrival Time for Process " << i + 1 << ": ";
        cin >> at;
        cout << "Enter Burst Time for Process " << i + 1 << ": ";
        cin >> bt;
        cout << "Enter Priority for Process " << i + 1 << ": ";
        cin >> pr;
        processes.push_back(Process(i + 1, at, bt, pr));
    }

    priorityScheduling(processes);

    cout << "\nProcess\tAT\tBT\tPriority\tWT\tTAT\tCT\n";
    float totalWT = 0, totalTAT = 0;

    for (auto &p : processes) {
        totalWT += p.wt;
        totalTAT += p.tat;
        cout << "P" << p.pid << "\t" << p.at << "\t" << p.bt << "\t" 
             << p.priority << "\t\t" << p.wt << "\t" 
             << p.tat << "\t" << p.ct << "\n";
    }

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << endl;

    return 0;
}
