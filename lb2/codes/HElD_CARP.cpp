#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <fstream>
#include <bitset>

using namespace std;

const int INF = INT_MAX / 2;

int n;
vector<vector<int>> graph;
vector<vector<int>> dp;
vector<vector<int>> parent;

ofstream logFile;

string toBinary(int num, int bits) {
    string result(bits, '0');
    for (int i = 0; i < bits; i++) {
        if (num & (1 << (bits - 1 - i))) {
            result[i] = '1';
        }
    }
    return result;
}

int tsp(int mask, int pos) {

    logFile << "tsp(" << mask << "," << pos << ") " << "mask: " << toBinary(mask, n) << endl;
    
    if (mask == (1 << n) - 1) {
        int ret = (graph[pos][0] != 0) ? graph[pos][0] : INF;
        logFile << "  return = " << ret << "\n";
        return ret;
    }
    
    if (dp[mask][pos] != -1) {
        logFile << "  memo = " << dp[mask][pos] << "\n";
        return dp[mask][pos];
    }
    
    int min_cost = INF;
    int best_next = -1;
    
    for (int next = 0; next < n; next++) {
        if (!(mask & (1 << next)) && graph[pos][next] != 0) {
            int new_mask = mask | (1 << next);
            int cost = graph[pos][next] + tsp(new_mask, next);
            
            if (cost < min_cost) {
                min_cost = cost;
                best_next = next;
                logFile << "    best " << next << " cost = " << min_cost << "\n";
            }
        }
    }
    
    dp[mask][pos] = min_cost;
    if (best_next != -1) parent[mask][pos] = best_next;
    
    logFile << "  dp[" << mask << "][" << pos << "] = " << min_cost << " parent = " << best_next << " mask: " << toBinary(mask, n) << "\n";
    
    return min_cost;
}


vector<int> reconstruct_path() {
    const int k = n;
    logFile << "Build result:" << endl;
    vector<int> path;
    int mask = 1;
    int pos = 0;
    path.push_back(0);
    logFile << "vertex: " << pos << " mask: " << toBinary(mask, n) << endl;
    while (mask != (1 << n) - 1) {
        int next = parent[mask][pos];
        
        if (next == -1) break;
        path.push_back(next);
        logFile << "next: " << next << endl;
        mask |= (1 << next);
        pos = next;
    }
    path.push_back(0);
    return path;
}

int main() {

    logFile.open("log.txt");
    
    cin >> n;
    graph.resize(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> graph[i][j];
    
    dp.resize(1 << n, vector<int>(n, -1));
    parent.resize(1 << n, vector<int>(n, -1));
    
    int result = tsp(1, 0);
    logFile << "Result: " << (result >= INF ? "no path" : to_string(result)) << "\n";
    
    if (result >= INF) {
        cout << "no path" << endl;
    } else {
        cout << result << endl;
        vector<int> path = reconstruct_path();
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i != path.size() - 1) cout << " ";
        }
        cout << endl;
        logFile << "Path: ";
        for (size_t i = 0; i < path.size(); i++) logFile << path[i] << " ";
        logFile << "\n";
    }
    
    logFile.close();
    return 0;
}