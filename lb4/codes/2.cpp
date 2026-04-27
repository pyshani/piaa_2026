#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

int main() {
    bool logFlag = true;
    
    ofstream logFile;
    if (logFlag) {
        logFile.open("log2.txt");
        if (!logFile.is_open()) {
            cerr << "Cannot open log file" << endl;
            return 1;
        }
    }

    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string A, B;
    getline(cin, A);
    getline(cin, B);
    
    if (logFlag) {
        logFile << "Проверка, является ли A циклическим сдвигом B" << endl;
        logFile << "A = \"" << A << "\" (длина " << A.size() << ")" << endl;
        logFile << "B = \"" << B << "\" (длина " << B.size() << ")" << endl << endl;
    }
    
    int n = A.size();
    int m = B.size();
    
    if (n != m) {
        if (logFlag) logFile << "Длины не совпадают: " << n << " != " << m << " -> -1" << endl;
        cout << -1 << endl;
        return 0;
    }
    
    if (n == 0) {
        if (logFlag) logFile << "Обе строки пусты -> индекс 0" << endl;
        cout << 0 << endl;
        return 0;
    }
    
    string S = A + A;
    if (logFlag) logFile << "Строим S = A + A = \"" << S << "\"" << endl << endl;
    
    if (logFlag) logFile << "Построение префикс-функции для B" << endl;
    vector<int> pi(m, 0);
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        if (logFlag) logFile << "i = " << i << ", символ B[" << i << "] = '" << B[i] << "', начальное j = " << j << endl;
        while (j > 0 && B[i] != B[j]) {
            if (logFlag) logFile << "  B[" << i << "] != B[" << j << "] (" << B[i] << " != " << B[j] 
                    << "), уменьшаем j = pi[" << j-1 << "] = " << pi[j-1] << endl;
            j = pi[j - 1];
        }
        if (B[i] == B[j]) {
            if (logFlag) logFile << "  B[" << i << "] == B[" << j << "] (" << B[i] << " == " << B[j] 
                    << "), увеличиваем j до " << j+1 << endl;
            ++j;
        } else {
            if (logFlag) logFile << "  j = 0, сравнение не требуется" << endl;
        }
        pi[i] = j;
        if (logFlag) logFile << "  pi[" << i << "] = " << j << endl << endl;
    }
    
    if (logFlag) {
        logFile << "Префикс-функция pi = [";
        for (int i = 0; i < m; ++i) {
            logFile << pi[i];
            if (i != m-1) logFile << ", ";
        }
        logFile << "]" << endl << endl;
    }
    
    if (logFlag) logFile << "Поиск B в S (S = A+A)" << endl;
    int j = 0;
    for (int i = 0; i < (int)S.size(); ++i) {
        if (logFlag) logFile << "Шаг " << i << ": S[" << i << "] = '" << S[i] << "', текущее j = " << j << endl;
        int step = 0;
        while (j > 0 && S[i] != B[j]) {
            if (logFlag) logFile << "  Попытка " << ++step << ": S[" << i << "] != B[" << j << "] (" 
                    << S[i] << " != " << B[j] << "), j = pi[" << j-1 << "] = " << pi[j-1] << endl;
            j = pi[j - 1];
        }
        if (S[i] == B[j]) {
            if (logFlag) logFile << "  S[" << i << "] == B[" << j << "] (" << S[i] << " == " << B[j] 
                    << "), увеличиваем j до " << j+1 << endl;
            ++j;
        } else {
            if (logFlag) logFile << "  j = 0, символы не сравниваются" << endl;
        }
        if (j == m) {
            int pos = i - m + 1;
            if (logFlag) logFile << "  Найдено совпадение B в S на позиции " << pos << " (индексы " 
                    << pos << "..." << i << ")" << endl;
            if (pos < n) {
                if (logFlag) logFile << "  Позиция " << pos << " < n = " << n << " -> это циклический сдвиг." << endl;
                if (logFlag) logFile << "Результат: " << pos << endl;
                cout << pos << endl;
                return 0;
            } else {
                if (logFlag) logFile << "  Позиция " << pos << " >= n = " << n << ", это перекрытие через границу, продолжаем поиск." << endl;
            }
            if (logFlag) logFile << "  Обновляем j = pi[" << m-1 << "] = " << pi[m-1] << endl;
            j = pi[j - 1];
        }
        if (logFlag) logFile << "  Новое j = " << j << endl << endl;
    }
    
    if (logFlag) logFile << "Совпадение не найдено в пределах первых n символов S -> -1" << endl;
    cout << -1 << endl;
    return 0;
}