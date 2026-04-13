#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstring>

using namespace std;

ofstream logFile;
bool logOpened = false;

void log_to_file(const string& msg) {
    if (logOpened) {
        logFile << msg;
    }
}

void print_dp_matrix(const vector<vector<int>>& dp, const string& s1, const string& s2) {
    int m = s1.length();
    int n = s2.length();
    
    log_to_file("\nТаблица dp (длины НОП для префиксов):\n");
    log_to_file("   ");
    log_to_file("0  ");
    for (int j = 0; j < n; ++j) {
        log_to_file(string(1, s2[j]) + "  ");
    }
    log_to_file("\n");
    
    for (int i = 0; i <= m; ++i) {
        if (i == 0) log_to_file("0  ");
        else log_to_file(string(1, s1[i-1]) + "  ");
        
        for (int j = 0; j <= n; ++j) {
            if (dp[i][j] > 9) log_to_file(to_string(dp[i][j]) + " ");
            else log_to_file(to_string(dp[i][j]) + "  ");
        }
        log_to_file("\n");
    }
    log_to_file("\n");
}

int main() {
    logFile.open("lcs_log.txt");
    if (!logFile.is_open()) {
        cerr << "Warning: не удалось открыть лог-файл\n";
        logOpened = false;
    } else {
        logOpened = true;
    }
    
    string s1, s2;
    getline(cin, s1);
    getline(cin, s2);
    
    log_to_file("Строка 1: " + s1 + "\n");
    log_to_file("Строка 2: " + s2 + "\n");
    
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    log_to_file("\nЗаполнение таблицы динамического программирования\n");
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
                log_to_file("Сравниваем '" + string(1, s1[i-1]) + "' и '" + string(1, s2[j-1]) + "' -> равны, берём dp[" +
                          to_string(i-1) + "][" + to_string(j-1) + "] + 1 = " + to_string(dp[i][j]) + "\n");
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                log_to_file("Сравниваем '" + string(1, s1[i-1]) + "' и '" + string(1, s2[j-1]) + "' -> не равны, max(dp[" +
                          to_string(i-1) + "][" + to_string(j) + "]=" + to_string(dp[i-1][j]) + ", dp[" +
                          to_string(i) + "][" + to_string(j-1) + "]=" + to_string(dp[i][j-1]) + ") = " + to_string(dp[i][j]) + "\n");
            }
        }
        log_to_file("\n После обработки строки " + to_string(i) + " (символ '" + s1[i-1] + "') \n");
        print_dp_matrix(dp, s1, s2);
    }
    
    log_to_file("\nТаблица полностью заполнена\n");
    print_dp_matrix(dp, s1, s2);
    
    log_to_file("\nВосстановление наибольшей общей подпоследовательности\n");
    string lcs = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        log_to_file("Текущие индексы: i=" + to_string(i) + " (символ s1[" + to_string(i-1) + "]='" + s1[i-1] + "'), j=" +
                  to_string(j) + " (символ s2[" + to_string(j-1) + "]='" + s2[j-1] + "')\n");
        if (s1[i-1] == s2[j-1]) {
            log_to_file("  Символы равны: добавляем '" + string(1, s1[i-1]) + "' в НОП и переходим по диагонали\n");
            lcs.push_back(s1[i-1]);
            --i; --j;
        } else if (dp[i-1][j] > dp[i][j-1]) {
            log_to_file("  Символы не равны, dp[" + to_string(i-1) + "][" + to_string(j) + "]=" + to_string(dp[i-1][j]) +
                      " > dp[" + to_string(i) + "][" + to_string(j-1) + "]=" + to_string(dp[i][j-1]) + " -> идём вверх\n");
            --i;
        } else {
            log_to_file("  Символы не равны, dp[" + to_string(i) + "][" + to_string(j-1) + "]=" + to_string(dp[i][j-1]) +
                      " >= dp[" + to_string(i-1) + "][" + to_string(j) + "]=" + to_string(dp[i-1][j]) + " -> идём влево\n");
            --j;
        }
    }
    reverse(lcs.begin(), lcs.end());
    
    log_to_file("\nРезультат\n");
    log_to_file("Наибольшая общая подпоследовательность: \"" + lcs + "\"\n");
    log_to_file("Длина НОП: " + to_string(lcs.length()) + "\n");
    
    if (logOpened) {
        logFile.close();
    }
    
    cout << lcs << "\n" << lcs.length() << endl;
    
    return 0;
}

//g++ .\main.cpp && Get-Content ./tests/test4.txt | .\a.exe