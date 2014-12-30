#include <emscripten.h>
#include <climits>
#include <cstdio>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#define inf 1000000000

using namespace std;

struct state {
    vector<int> a;
    int nr_moves;
    int idx;
    int previous;
    int i, j;
    state(int _nr_moves, vector<int> _a, int _idx, int _i = 0, int _j = 0) {
        nr_moves = _nr_moves; a = _a; idx = _idx; i = _i; j = _j;
    }
};

extern "C" {
void solve(int scale, int source, int sink, int goal, int where,
    int cap1, int maxes1,
    int cap2, int maxes2,
    int cap3, int maxes3,
    int cap4, int maxes4,
    int cap5, int maxes5) {
    
    int n = 7;

    int maxes[n];
    // source first
    maxes[0] = inf;
    maxes[1] = maxes1; maxes[2] = maxes2; maxes[3] = maxes3; maxes[4] = maxes4; maxes[5] = maxes5;
    // sink last
    maxes[6] = inf;

    int cap[n];
    cap[0] = inf; cap[1] = cap1; cap[2] = cap2; cap[3] = cap3; cap[4] = cap4; cap[5] = cap5; cap[6] = 0;

    for (int i = 0; i < n; ++i) {
        if (maxes[i] == where) {
            where = i;
            break;
        }
    }
    
    vector<int> a;
    for (int i = 0; i < n; ++i) {
        a.push_back(cap[i]);
    }
    
    queue<state> kju;
    int idx = 0;
    state s = state(0, a, idx);
    kju.push(s);
    vector<state> all_states;
    all_states.push_back(s);
    set<string> bio;
    bool found_sol = false;
    while (!kju.empty()) {
        s = kju.front(); kju.pop();
        
        if (s.a[where] == goal) { found_sol = true; break; }
        
        string key = "";
        char st[10];
        for (int i = 1; i < n-1; ++i) {
            sprintf(st, "%5d#", s.a[i]);
            key += string(st);
        }

        if (bio.find(key) != bio.end()) continue;
        bio.insert(key);
        
        for (int i = 1-source; i < n-1; ++i) {
            for (int j = 1; j < n-1+sink; ++j) {
                if (i == j) continue;
                
                // from glass #i to glass #j
                if (s.a[i] != 0 && s.a[j] != maxes[j]) {
                    int litres = min(s.a[i], maxes[j] - s.a[j]);
                    a = s.a;
                    a[i] -= litres;
                    a[j] += litres;
                    if (i != 0) {
                        sprintf(st, "%5d#", a[i]);
                        key.replace(i*6, 6, string(st));
                    }
                    if (j < n-1) {
                        sprintf(st, "%5d#", a[j]);
                        key.replace(j*6, 6, string(st));
                    }
                    
                    if (bio.find(key) == bio.end()) {
                        state new_state = state(s.nr_moves+1, a, ++idx, i, j);
                        new_state.previous = s.idx;
                        all_states.push_back(new_state);
                        kju.push(new_state);
                    }
                    
                    if (i != 0) {
                        sprintf(st, "%5d#", s.a[i]);
                        key.replace(i*6, 6, string(st));
                    }
                    if (j < n-1) {
                        sprintf(st, "%5d#", s.a[j]);
                        key.replace(j*6, 6, string(st));
                    }
                }
            }

            if (i != 0 && s.a[i] > 0 && s.a[i] % 2 == 0 && scale) {
                a = s.a;
                a[i] /= 2;

                sprintf(st, "%5d#", a[i]);
                key.replace(i*6, 6, string(st));
                
                if (bio.find(key) != bio.end()) continue;
                bio.insert(key);

                state new_state = state(s.nr_moves+1, a, ++idx, i, i);
                new_state.previous = s.idx;
                all_states.push_back(new_state);
                kju.push(new_state);
                
                sprintf(st, "%5d#", s.a[i]);
                key.replace(i*6, 6, string(st));
            }
        }
    }

    if (found_sol) {
        stack<state> plates;
        while (s.idx) {
            plates.push(s);
            s = all_states[s.previous];
        }

        while (!plates.empty()) {
            state s = plates.top(); plates.pop();
            if (s.i == 0) printf("Fill: %d\n", maxes[s.j]);
            else if (s.j == n-1) printf("Empty: %d\n", maxes[s.i]);
            else if (s.i == s.j) printf("Half: %d\n", maxes[s.i]);
            else printf("From: %d to: %d\n", maxes[s.i], maxes[s.j]);
        }
    }
}
}

int main() {
    // scale, source
    solve(0, 1,
    // sink, goal, where
          1,   1,    3,
    // cap1, maxes1
          0, 2,
    // cap2, maxes2
          0, 3,
    // cap3, maxes3
          0, 0,
    // cap4, maxes4
          0, 0,
    // cap5, maxes5
          0, 0);
    return 0;
}





























