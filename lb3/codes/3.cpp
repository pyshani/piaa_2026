#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

bool log_flag = true;

void print_table(ofstream& log, const vector<vector<int>>& dp,
                 const string& A, const string& B) {
    int n = A.size(), m = B.size();
    log << "    ";
    for (int j = 0; j < m; ++j) log << "  " << B[j];
    log << "\n";
    for (int i = 0; i <= n; ++i) {
        if (i == 0) log << " ";
        else log << A[i - 1];
        for (int j = 0; j <= m; ++j)
            log << setw(3) << dp[i][j];
        log << "\n";
    }
    log << "\n";
}

int main() {
    string s, t;
    cin >> s >> t;
    int n = s.size(), m = t.size();

    const int costR = 1, costI = 1, costD = 1;

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) dp[i][0] = i * costD;
    for (int j = 1; j <= m; ++j) dp[0][j] = j * costI;

    ofstream log;
    if (log_flag) {
        log.open("log3.txt");
        log << "стоимость_замены=" << costR << " стоимость_вставки=" << costI
            << " стоимость_удаления=" << costD << "\n";
        log << "Строка A=" << s << " Строка B=" << t << "\n\n";
        log << "Начальная таблица:\n";
        print_table(log, dp, s, t);
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            char ca = s[i - 1], cb = t[j - 1];
            if (ca == cb) {
                dp[i][j] = dp[i - 1][j - 1];
                if (log_flag) {
                    log << "Сравнение '" << ca << "' и '" << cb
                        << "' -> равны, берём dp[" << i - 1 << "][" << j - 1
                        << "] = " << dp[i][j] << "\n";
                }
            } else {
                int repl = dp[i - 1][j - 1] + costR;
                int ins  = dp[i][j - 1] + costI;
                int del  = dp[i - 1][j] + costD;
                dp[i][j] = min({repl, ins, del});
                if (log_flag) {
                    log << "Сравнение '" << ca << "' и '" << cb
                        << "' -> не равны, min(замена=" << repl
                        << ", вставка=" << ins << ", удаление=" << del
                        << ") = " << dp[i][j] << "\n";
                }
            }
        }
        if (log_flag) {
            log << "\nПосле строки " << i << " (символ '" << s[i - 1] << "'):\n";
            print_table(log, dp, s, t);
        }
    }

    int distance = dp[n][m];
    if (log_flag) {
        log << "Финальное расстояние = " << distance << "\n";
        log.close();
    }

    cout << distance << endl;

    return 0;
}

//g++ .\3.cpp && Get-Content ./tests/test3.txt | .\a.exe