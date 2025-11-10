//FIFO

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int fifoPageReplacement(vector<int> pages, int capacity) {
    queue<int> q;           
    vector<int> memory;      
    int page_faults = 0;

    for (int page : pages) {
        if (find(memory.begin(), memory.end(), page) == memory.end()) {
            page_faults++;
            if (memory.size() == capacity) {
                int oldest = q.front();
                q.pop();
                memory.erase(find(memory.begin(), memory.end(), oldest));
            }
            memory.push_back(page);
            q.push(page);
        }
    }
    return page_faults;
}
int main() {
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int capacity = 3;
    cout << "Number of page faults (FIFO): "
         << fifoPageReplacement(pages, capacity) << endl;
    return 0;
}

//LRU

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int lruPageReplacement(vector<int> pages, int capacity) {
    vector<int> memory;   
    int page_faults = 0;
    for (int page : pages) {
        auto it = find(memory.begin(), memory.end(), page);
        if (it == memory.end()) { 
            page_faults++;
            if (memory.size() == capacity)
                memory.erase(memory.begin()); 
        } else {
            memory.erase(it); // move accessed page to most recent
        }
        memory.push_back(page);
    }
    return page_faults;
}
int main() {
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int capacity = 3;
    cout << "Number of page faults (LRU): "
         << lruPageReplacement(pages, capacity) << endl;
    return 0;
}

//OPT

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

int optimalPageReplacement(vector<int> pages, int capacity) {
    vector<int> memory;
    int page_faults = 0;
    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (find(memory.begin(), memory.end(), page) == memory.end()) {
            if (memory.size() == capacity) {
                int farthest = -1, index_to_replace = -1;

                for (int j = 0; j < memory.size(); j++) {
                    int next_use = numeric_limits<int>::max();

                    for (int k = i + 1; k < pages.size(); k++) {
                        if (pages[k] == memory[j]) {
                            next_use = k;
                            break;
                        }
                    }

                    if (next_use > farthest) {
                        farthest = next_use;
                        index_to_replace = j;
                    }
                }
                memory[index_to_replace] = page;
            } else {
                memory.push_back(page);
            }
            page_faults++;
        }
    }
    return page_faults;
}
int main() {
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int capacity = 3;
    cout << "Number of page faults (Optimal): "
         << optimalPageReplacement(pages, capacity) << endl;
    return 0;
}
