#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;


int main(){

    int start_ver;
    cin >> start_ver;
    cin.ignore();

    vector<vector<double>> matr_way;

    string line;

    while (std::getline(std::cin, line)) {

        vector<double> row;
        std::istringstream iss(line);
        double val;
        
        while (iss >> val) {
            row.push_back(val);
        }

        matr_way.push_back(row);

    }

    int N = matr_way.size();

    double good_len = 0;
    vector<bool> visited(N, false);
    vector<int> good_way;

    visited[start_ver - 1] = true;
    good_way.push_back(start_ver);

    int idx = start_ver - 1;

    for(int iter = 0; iter < N - 1; iter++){
        double min = INFINITY;
        int min_idx = 0;
        for(int j = 0; j < N; j++){
            if (matr_way[idx][j] < min and idx != j and !visited[j]){
                min_idx = j;
                min = matr_way[idx][j];
            }
        }
        cout << "min road to vertex: " << min_idx + 1 << " min value " << min << endl;

        visited[min_idx] = true;
        good_way.push_back(min_idx + 1);
        good_len += min;
        idx = min_idx;
    }

    idx = good_way[N - 1] - 1;
    good_way.push_back(start_ver);
    good_len += matr_way[idx][start_ver - 1];

    for(int i = 0; i <= N; i++){
        cout << good_way[i] << " ";
    }

    cout << endl << good_len << endl;
    

    return 0;
}