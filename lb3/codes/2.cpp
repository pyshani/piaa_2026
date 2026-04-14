#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>

bool log_flag = true;

void print_table(std::ofstream& log, const std::vector<std::vector<int>>& dp,
                 const std::string& A, const std::string& B) {
    int n = A.size(), m = B.size();
    log << "    ";
    for (int j = 0; j < m; ++j) log << "  " << B[j];
    log << "\n";
    for (int i = 0; i <= n; ++i) {
        if (i == 0) log << " ";
        else log << A[i - 1];
        for (int j = 0; j <= m; ++j)
            log << std::setw(3) << dp[i][j];
        log << "\n";
    }
    log << "\n";
}

int main() {
    int costR, costI, costD;
    std::cin >> costR >> costI >> costD;
    std::string A, B;
    std::cin >> A >> B;
    int n = A.size();
    int m = B.size();

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) dp[i][0] = i * costD;
    for (int j = 1; j <= m; ++j) dp[0][j] = j * costI;

    std::ofstream log("log2.txt");
    if (log_flag) {
        log << "стоимость_замены=" << costR << " стоимость_вставки=" << costI
            << " стоимость_удаления=" << costD << "\n";
        log << "Строка A=" << A << " Строка B=" << B << "\n\n";
        log << "Начальная таблица:\n";
        print_table(log, dp, A, B);
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            char ca = A[i - 1], cb = B[j - 1];
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
                dp[i][j] = std::min({repl, ins, del});
                if (log_flag) {
                    log << "Сравнение '" << ca << "' и '" << cb
                        << "' -> не равны, min(замена=" << repl
                        << ", вставка=" << ins << ", удаление=" << del
                        << ") = " << dp[i][j] << "\n";
                }
            }
        }
        if (log_flag) {
            log << "\nПосле строки " << i << " (символ '" << A[i - 1] << "'):\n";
            print_table(log, dp, A, B);
        }
    }

    if (log_flag) {
        log << "Финальное расстояние = " << dp[n][m] << "\n";
        log << "Восстановление пути: \n";
    }

    std::string ops;
    int i = n, j = m;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && A[i - 1] == B[j - 1] && dp[i][j] == dp[i - 1][j - 1]) {
            ops.push_back('M');
            if (log_flag) {
                log << "A[i - 1] == B[j - 1] " << A[i - 1] << " == " << B[j - 1] << 
                " dp[i][j] == dp[i - 1][j - 1] " << dp[i][j] << " == " <<  dp[i - 1][j - 1] << "\n" << 
                "i--; j--;" << "\n";
                log << "M" << "\n";
            }
            i--; j--;
        } else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + costR) {
            ops.push_back('R');
            if (log_flag) {
                log << "dp[i][j] == dp[i - 1][j - 1] + costR " << dp[i][j] << " == " << dp[i - 1][j - 1] + costR << "\n" 
                << "i--; j--;" << "\n";
                log << "R" << "\n";
            }
            i--; j--;
        } else if (j > 0 && dp[i][j] == dp[i][j - 1] + costI) {
            ops.push_back('I');
            if (log_flag) {
                log << "dp[i][j] == dp[i][j - 1] + costI " << dp[i][j] << " == " <<  dp[i][j - 1] + costI << "\n" 
                << "j--;" << "\n";
                log << "I" << "\n";
            }
            j--;
        } else if (i > 0 && dp[i][j] == dp[i - 1][j] + costD) {
            ops.push_back('D');
            if (log_flag) {
                log << "dp[i][j] == dp[i - 1][j] + costD " << dp[i][j] << " == " <<  dp[i - 1][j] + costD << "\n" 
                << "i--;" << "\n";
                log << "D" << "\n";
            }
            i--;
        }
    }
    std::reverse(ops.begin(), ops.end());

    std::cout << ops << std::endl;
    std::cout << A << std::endl;
    std::cout << B << std::endl;

    log.close();

    return 0;
}

//g++ .\2.cpp && Get-Content ./tests/test2.txt | .\a.exe