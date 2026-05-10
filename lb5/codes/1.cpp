#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <cstring>
#include <fstream>

using namespace std;

struct Node {
    int next[5];
    int fail;
    int out;          
    vector<int> pat;
    Node() {
        memset(next, -1, sizeof(next));
        fail = 0;
        out = -1;
    }
};

int char_to_idx(char c) {
    switch (c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        case 'N': return 4;
        default: return -1;
    }
}

char idx_to_char(int i) {
    switch (i) {
        case 0: return 'A';
        case 1: return 'C';
        case 2: return 'G';
        case 3: return 'T';
        case 4: return 'N';
        default: return '?';
    }
}


void print_node(ofstream& out, const vector<Node>& nodes, int v, const string& label = "") {
    if (!label.empty()) out << label << " ";
    out << "Node " << v << ":\n";
    out << "  next: ";
    for (int c = 0; c < 5; ++c) {
        out << idx_to_char(c) << "->" << nodes[v].next[c];
        if (c < 4) out << ", ";
    }
    out << "\n  fail = " << nodes[v].fail;
    out << "\n  out  = " << nodes[v].out;
    out << "\n  pat  = [ ";
    for (int p : nodes[v].pat) out << p << " ";
    out << "]\n";
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool logfile = true;

    ofstream logfile_file;
    if (logfile) {
        logfile_file.open("log1.txt");
        if (!logfile_file.is_open()) {
            cerr << "Не удалось открыть logfile_output.txt для записи\n";
            return 1;
        }
        logfile_file << "Отладочный вывод: \n\n";
    }

    string T;
    cin >> T;
    int n;
    cin >> n;
    vector<int> pattern_len(n + 1);
    vector<Node> nodes;
    nodes.reserve(225000);
    nodes.push_back(Node());

    if (logfile) {
        logfile_file << "Текст T: " << T << "\n";
        logfile_file << "Количество паттернов: " << n << "\n";
        logfile_file << "Паттерны:\n";
    }

    for (int idx = 1; idx <= n; ++idx) {
        string p;
        cin >> p;
        pattern_len[idx] = (int)p.size();
        if (logfile) logfile_file << "  " << idx << ": " << p << "\n";

        int v = 0;
        for (char ch : p) {
            int c = char_to_idx(ch);
            if (c == -1) {
                cout << "Error: '" << ch <<  "' symbol not in alfabet" << endl;
                return 0;
            }
            if (nodes[v].next[c] == -1) {
                nodes.push_back(Node());
                nodes[v].next[c] = (int)nodes.size() - 1;
                if (logfile) logfile_file << "    Создан новый узел " << nodes.size()-1
                                      << " по переходу " << idx_to_char(c) << " из узла " << v << "\n";
            }
            v = nodes[v].next[c];
        }
        nodes[v].pat.push_back(idx);
        if (logfile) logfile_file << "    Паттерн " << idx << " добавлен в узел " << v << "\n";
    }

    if (logfile) {
        logfile_file << "\nБор после вставки \n";
        for (size_t v = 0; v < nodes.size(); ++v) {
            print_node(logfile_file, nodes, v);
        }
    }

    queue<int> q;
    for (int c = 0; c < 5; ++c) {
        if (nodes[0].next[c] != -1) {
            int v = nodes[0].next[c];
            nodes[v].fail = 0;
            nodes[v].out = -1;
            q.push(v);
            if (logfile) logfile_file << "Инициализация: узел " << v
                                  << " (переход по " << idx_to_char(c) << ") fail=0, out=-1\n";
        } else {
            nodes[0].next[c] = 0;
        }
    }

    if (logfile) logfile_file << "\n Построение fail-ссылок \n";

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (logfile) logfile_file << "Обрабатываем узел " << u << "\n";

        for (int c = 0; c < 5; ++c) {
            if (nodes[u].next[c] != -1) {
                int v = nodes[u].next[c];
                nodes[v].fail = nodes[nodes[u].fail].next[c];
                if (!nodes[nodes[v].fail].pat.empty())
                    nodes[v].out = nodes[v].fail;
                else
                    nodes[v].out = nodes[nodes[v].fail].out;

                if (logfile) {
                    logfile_file << "  Переход по " << idx_to_char(c) << " -> узел " << v
                               << ": fail = " << nodes[v].fail
                               << ", out = " << nodes[v].out << "\n";
                }
                q.push(v);
            } else {
                nodes[u].next[c] = nodes[nodes[u].fail].next[c];
                if (logfile && nodes[u].next[c] != 0) {
                    logfile_file << "  Переназначен переход " << idx_to_char(c)
                               << " узла " << u << " -> " << nodes[u].next[c] << "\n";
                }
            }
        }
    }

    if (logfile) {
        logfile_file << "\n Финальное состояние автомата \n";
        for (size_t v = 0; v < nodes.size(); ++v) {
            print_node(logfile_file, nodes, v);
        }
    }

    vector<vector<int>> ans(T.size() + 1);
    int state = 0;

    if (logfile) logfile_file << "\n обход текста \n";

    for (int i = 0; i < (int)T.size(); ++i) {
        char ch = T[i];
        int c = char_to_idx(ch);
        if (c == -1) {
            cout << "Error: '" << ch <<  "' symbol not in alfabet" << endl;
            return 0;
        }
        int old_state = state;
        state = nodes[state].next[c];

        if (logfile) {
            logfile_file << "Позиция " << i+1 << " (" << ch << "): "
                       << "состояние " << old_state << " -> " << state << "\n";
        }

        int pos = i + 1;

        if (!nodes[state].pat.empty()) {
            if (logfile) logfile_file << "  Найдены паттерны в узле " << state << ": ";
            for (int idx : nodes[state].pat) {
                int start = pos - pattern_len[idx] + 1;
                ans[start].push_back(idx);
                if (logfile) logfile_file << idx << " (старт " << start << ") ";
            }
            if (logfile && !nodes[state].pat.empty()) logfile_file << "\n";
        }

        int out = nodes[state].out;
        int depth = 0;
        while (out != -1) {
            if (!nodes[out].pat.empty()) {
                if (logfile) logfile_file << "  out[" << depth << "] = " << out << " -> паттерны: ";
                for (int idx : nodes[out].pat) {
                    int start = pos - pattern_len[idx] + 1;
                    ans[start].push_back(idx);
                    if (logfile) logfile_file << idx << " (старт " << start << ") ";
                }
                if (logfile && !nodes[out].pat.empty()) logfile_file << "\n";
            }
            out = nodes[out].out;
            ++depth;
        }
    }

    if (logfile) {
        logfile_file << "\nРезультаты поиска\n";
    }

    for (int i = 1; i <= (int)T.size(); ++i) {
        if (!ans[i].empty()) {
            sort(ans[i].begin(), ans[i].end());
            for (int idx : ans[i]) {
                cout << i << " " << idx << '\n';
                if (logfile) logfile_file << i << " " << idx << '\n';
            }
        }
    }

    if (logfile) {
        logfile_file << "\nКонец \n";
        logfile_file.close();
    }

    return 0;
}