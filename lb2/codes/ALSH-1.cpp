#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;

const double INF = numeric_limits<double>::infinity();

double compute_L(const vector<bool>& visited, const vector<int>& path, const vector<vector<double>>& matr) {
    int N = matr.size();
    double total = 0.0;

    int start = path.front();
    int end = path.back();

    double min_in = INF;
    double min_out = INF;
    for (int u = 0; u < N; ++u) {
        if (u != start && u != end) {
            double w = matr[u][start];
            if (w < min_in && w != 0) min_in = w;
            w = matr[end][u];
            if (w < min_out && w != 0) min_out = w;
        }
    }

    total += min_in + min_out;

    return total / 2.0;
}

int main() {
    int start_ver;
    cin >> start_ver;
    cin.ignore();
    start_ver = 0;

    vector<vector<double>> matr;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        vector<double> row;
        istringstream iss(line);
        double val;
        while (iss >> val) {
            row.push_back(val);
        }
        matr.push_back(row);
    }

    int N = matr.size();
    if (N == 0) {
        cout << "no path" << endl;
        return 0;
    }

    int start_idx = start_ver;

    vector<bool> visited(N, false);
    vector<int> path;
    path.push_back(start_idx);
    visited[start_idx] = true;
    
    double current_cost = 0.0;
    int last = start_idx;

    ofstream logFile("logALSH.txt");

    logFile << "\nНачало алгоритма\n";
    logFile << "Стартовая вершина: " << start_ver << "\n\n";

    while (path.size() < N) {
        logFile << "Шаг " << path.size() << endl;
        logFile << "Текущий путь: ";
        for (int v : path) logFile << v << " ";
        logFile << "\nТекущая концевая вершина: " << last << "\n";

        if (path.size() == N - 1) {
            int next = -1;
            for (int i = 0; i < N; ++i) {
                if (!visited[i]) {
                    next = i;
                    break;
                }
            }
            if (next == -1) {
                logFile << "Ошибка: не найдена непосещённая вершина" << endl;
                cout << "no path" << endl;
                return 0;
            }
            double s = matr[last][next];
            if (s == 0) {
                logFile << "Нет ребра из " << last << " в " << next << endl;
                cout << "no path" << endl;
                return 0;
            }
            logFile << "Последняя вершина: " << next << ", вес: " << s << "\n";
            path.push_back(next);
            visited[next] = true;
            current_cost += s;
            last = next;
            break;
        }

        double best_value = INF;
        int best_next = -1;

        logFile << "Рассматриваем кандидатов:\n";
        for (int j = 0; j < N; ++j) {
            if (!visited[j]) {
                double s = matr[last][j];
                if(s == 0) continue;
                vector<bool> new_visited = visited;
                new_visited[j] = true;

                vector<int> new_path = path;
                new_path.push_back(j);
                double L = compute_L(new_visited, new_path, matr);
                double value = s + L;

                logFile << "Вершина " << j << ": s=" << s << ", L=" << L << ", s+L=" << value;
                if (value < best_value) logFile << " (лучший пока)";
                logFile << "\n";

                if (value < best_value) {
                    best_value = value;
                    best_next = j;
                }
            }
        }

        if (best_next == -1) {
            logFile << "Нет доступных вершин для продолжения пути из " << last << endl;
            cout << "no path" << endl;
            return 0;
        }

        double s = matr[last][best_next];
        logFile << "\nВыбрана вершина " << best_next << " (s=" << s << ", s+L=" << best_value << ")\n";
        path.push_back(best_next);
        visited[best_next] = true;
        current_cost += s;
        last = best_next;
        logFile << "Текущая стоимость пути: " << current_cost << "\n\n";
    }

    double closing_cost = matr[last][start_idx];
    if (closing_cost == 0) {
        logFile << "Нет замыкающего ребра из " << last << " в " << start_idx << endl;
        cout << "no path" << endl;
        return 0;
    }
    current_cost += closing_cost;
    logFile << "Замыкающая дуга: " << last << " -> " << start_ver << ", вес: " << closing_cost << "\n";
    logFile << "Полный путь: ";
    for (int v : path) logFile << v << " ";
    logFile << start_ver << "\n";
    logFile << "Общая стоимость: " << current_cost << endl;

    logFile.close();

    for (int v : path) {
        cout << v << " ";
    }
    cout << start_ver << endl;
    cout << current_cost << endl;

    return 0;
}