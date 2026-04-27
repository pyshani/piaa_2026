#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

int main() {

    bool logFlag = true;

    ofstream logFile;
    if (logFlag) {
        logFile.open("log1.txt");
        if (!logFile.is_open()) {
            cerr << "Не удалось создать лог-файл!" << endl;
            return 1;
        }
    }

    string P, T;
    getline(cin, P);
    getline(cin, T);

    if (logFlag) {
        logFile << "Алгоритм Кнута-Морриса-Пратта" << endl;
        logFile << "Шаблон P: \"" << P << "\" (длина " << P.size() << ")" << endl;
        logFile << "Текст   T: \"" << T << "\" (длина " << T.size() << ")" << endl << endl;
    }

    int m = (int)P.size();
    if (m == 0) {
        if (logFlag) logFile << "Шаблон пуст, выход с -1." << endl;
        cout << -1 << endl;
        return 0;
    }

    if (logFlag) logFile << "Построение префикс-функции" << endl;
    vector<int> pi(m, 0);
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        if (logFlag) logFile << "i = " << i << ", j = " << j << ", P[i]='" << P[i] << "', P[j]='" << P[j] << "'" << endl;
        while (j > 0 && P[i] != P[j]) {
            if (logFlag) logFile << "  несовпадение, уменьшаем j = pi[" << j-1 << "] = " << pi[j-1] << endl;
            j = pi[j - 1];
        }
        if (P[i] == P[j]) {
            ++j;
            if (logFlag) logFile << "  совпадение, новое j = " << j << endl;
        }
        pi[i] = j;
        if (logFlag) logFile << "  pi[" << i << "] = " << j << endl;
    }

    if (logFlag) {
        logFile << "Префикс-функция: [";
        for (int i = 0; i < m; ++i) {
            logFile << pi[i];
            if (i != m-1) logFile << ", ";
        }
        logFile << "]" << endl << endl;
        logFile << "Поиск вхождений" << endl;
    }

    vector<int> matches;
    int j = 0;
    for (int i = 0; i < (int)T.size(); ++i) {
        if (logFlag) logFile << "T[" << i << "] = '" << T[i] << "', j = " << j << endl;

        while (j > 0 && T[i] != P[j]) {
            if (logFlag) logFile << "  несовпадение, переходим j = pi[" << j-1 << "] = " << pi[j-1] << endl;
            j = pi[j - 1];
        }
        if (T[i] == P[j]) {
            ++j;
            if (logFlag) logFile << "  совпадение, j = " << j << endl;
        }

        if (j == m) {
            int start = i - m + 1;
            matches.push_back(start);
            if (logFlag) logFile << "  найдено вхождение на позиции " << start << endl;
            j = pi[j - 1];
            if (logFlag) logFile << "  после обработки j = " << j << endl;
        }
    }

    if (logFlag) logFile << "Результат" << endl;
    if (matches.empty()) {
        if (logFlag) logFile << "Шаблон не найден." << endl;
        cout << -1 << endl;
    } else {
        if (logFlag) {
            logFile << "Найденные индексы: ";
            for (size_t k = 0; k < matches.size(); ++k) {
                if (k > 0) logFile << ",";
                logFile << matches[k];
            }
            logFile << endl;
        }
        for (size_t k = 0; k < matches.size(); ++k) {
            if (k > 0) cout << ",";
            cout << matches[k];
        }
        cout << endl;
    }

    if (logFlag) {
        logFile << "Завершение работы." << endl;
        logFile.close();
    }

    return 0;
}