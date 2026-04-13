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
        else log << A[i-1] << "";
        for (int j = 0; j <= m; ++j)
            log << std::setw(3) << dp[i][j];
        log << "\n";
    }
    log << "\n";
}

int main() {
    int cr, ci, cd;
    std::cin >> cr >> ci >> cd;
    std::string A, B;
    std::cin >> A >> B;
    int n = A.size(), m = B.size();
    
    std::vector<std::vector<int>> dp(n+1, std::vector<int>(m+1));
    for (int i = 0; i <= n; ++i) dp[i][0] = i * cd;
    for (int j = 0; j <= m; ++j) dp[0][j] = j * ci;

    std::ofstream log("log1.txt");
    
    if (log_flag) {    
        log << "стоимость_замены=" << cr << " стоимость_вставки=" << ci << " стоимость_удаления=" << cd << "\n";
        log << "Строка A=" << A << " Строка B=" << B << "\n\n";
        log << "Начальная таблица:\n";
        print_table(log, dp, A, B);
    }
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            char ca = A[i-1], cb = B[j-1];
            if (ca == cb) {
                dp[i][j] = dp[i-1][j-1];
                if (log_flag) log << "Сравнение '" << ca << "' и '" << cb << "' -> равны, берём dp[" << i-1 << "][" << j-1 << "] = " << dp[i][j] << "\n";
            } else {
                int repl = dp[i-1][j-1] + cr;
                int ins = dp[i][j-1] + ci;
                int del = dp[i-1][j] + cd;
                dp[i][j] = std::min({repl, ins, del});
                if (log_flag) log << "Сравнение '" << ca << "' и '" << cb << "' -> не равны, min(замена=" << repl << ", вставка=" << ins << ", удаление=" << del << ") = " << dp[i][j] << "\n";
            }
        }
        if (log_flag) {
            log << "\nПосле строки " << i << " (символ '" << A[i-1] << "'):\n";
            print_table(log, dp, A, B);
        }    
    }
    
    if (log_flag) log << "Финальное расстояние = " << dp[n][m] << "\n";
    log.close();
    
    std::cout << dp[n][m] << std::endl;
    return 0;
}

//g++ .\1.cpp && Get-Content ./tests/test1.txt | .\a.exe