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

int main() {
    return 0;
}

extern "C" {
void solve(int n, int scale, int maxes1, int maxes2, int maxes3, int maxes4, int maxes5, int cap, int where) {
    vector<state> all_states;
    int maxes[6];
    maxes[0] = maxes1;
    maxes[1] = maxes2;
    maxes[2] = maxes3;
    maxes[3] = maxes4;
    maxes[4] = maxes5;
    maxes[n] = INT_MAX;

    for (int i = 0; i < n; ++i) {
        if (maxes[i] == where) {
            where = i;
            break;
        }
    }
    
    vector<int> a = vector<int>(n, 0);
    a.push_back(inf);
    ++n;
    
    queue<state> kju;
    int idx = 0;
    state s = state(0, a, idx);
    kju.push(s);
    all_states.push_back(s);
    set<string> bio;
    while (!kju.empty()) {
        s = kju.front(); kju.pop();
        
        if (s.a[where] == cap) {
            stack<state> plates;
            while (s.idx) {
                plates.push(s);
                s = all_states[s.previous];
            }

            while (!plates.empty()) {
                state s = plates.top(); plates.pop();
                if (maxes[s.i] == INT_MAX) printf("Fill: %d\n", maxes[s.j]);
                else if (maxes[s.j] == INT_MAX) printf("Empty: %d\n", maxes[s.i]);
                else if (s.i == s.j) printf("Half: %d\n", maxes[s.i]);
                else printf("From: %d to: %d\n", maxes[s.i], maxes[s.j]);
            }
            return;
        }
        
        string key = "";
        char st[10];
        for (int i = 0; i < n-1; ++i) {
            sprintf(st, "%5d#", s.a[i]);
            key += string(st);
        }
        if (bio.find(key) != bio.end()) continue;
        bio.insert(key);
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                
                // from i to j
                if (s.a[i] != 0 && s.a[j] != maxes[j]) {
                    int litres = min(s.a[i], maxes[j] - s.a[j]);
                    a = s.a;
                    a[i] -= litres;
                    a[j] += litres;
                    if (i < n-1) {
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
                    
                    if (i < n-1) {
                        sprintf(st, "%5d#", s.a[i]);
                        key.replace(i*6, 6, string(st));
                    }
                    if (j < n-1) {
                        sprintf(st, "%5d#", s.a[j]);
                        key.replace(j*6, 6, string(st));
                    }
                }
            }

            if (i < n-1 && s.a[i] && s.a[i] % 2 == 0 && scale) {
                a = s.a;
                a[i] /= 2;

                sprintf(st, "%5d#", a[i]);
                key.replace(i*6, 6, string(st));
                
                if (bio.find(key) == bio.end()) {
                    state new_state = state(s.nr_moves+1, a, ++idx, i, i);
                    new_state.previous = s.idx;
                    all_states.push_back(new_state);
                    kju.push(new_state);
                }
                
                sprintf(st, "%5d#", s.a[i]);
                key.replace(i*6, 6, string(st));
            }
        }
    }
}
}
