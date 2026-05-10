#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <fstream>

using namespace std;

struct Node {
    int next[26];
    int fail;
    int out;
    bool term;
    Node() {
        fill(next, next + 26, -1);
        fail = -1;
        out = -1;
        term = false;
    }
};

int get_fail_len(int v, const vector<Node>& trie, vector<int>& memo) {
    if (v == 0) return 0;
    if (memo[v] != -1) return memo[v];
    memo[v] = 1 + get_fail_len(trie[v].fail, trie, memo);
    return memo[v];
}

int get_out_len(int v, const vector<Node>& trie, vector<int>& memo) {
    if (v == 0) return 0;
    if (trie[v].out == -1) return 0;
    if (memo[v] != -1) return memo[v];
    memo[v] = 1 + get_out_len(trie[v].out, trie, memo);
    return memo[v];
}

int main() {
    int log_flag;
    cin >> log_flag;
    ofstream log_file;
    if (log_flag) {
        log_file.open("log3.txt");
    }

    int n;
    cin >> n;
    vector<string> patterns(n);
    for (int i = 0; i < n; ++i) {
        cin >> patterns[i];
    }

    if (log_flag) {
        log_file << "начало построения бора\n";
        log_file << "количество образцов: " << n << "\n";
    }

    vector<Node> trie(1);

    for (int i = 0; i < n; ++i) {
        const string& p = patterns[i];
        if (log_flag) {
            log_file << "добавление образца " << p << "\n";
        }
        int v = 0;
        for (char ch : p) {
            int c = ch - 'a';
            if (trie[v].next[c] == -1) {
                trie[v].next[c] = trie.size();
                trie.emplace_back();
                if (log_flag) {
                    log_file << "  создана новая вершина " << trie.size() - 1 << " из " << v << " по букве " << ch << "\n";
                }
            }
            v = trie[v].next[c];
        }
        trie[v].term = true;
        if (log_flag) {
            log_file << "  вершина " << v << " помечена как терминальная\n";
        }
    }

    if (log_flag) {
        log_file << "\nбор после добавления всех образцов:\n";
        for (size_t v = 0; v < trie.size(); ++v) {
            log_file << "вершина " << v << ": переходы ";
            bool first = true;
            for (int c = 0; c < 26; ++c) {
                if (trie[v].next[c] != -1) {
                    if (!first) log_file << ", ";
                    log_file << char('a' + c) << " -> " << trie[v].next[c];
                    first = false;
                }
            }
            if (trie[v].term) log_file << " (терминальная)";
            log_file << "\n";
        }
    }

    queue<int> q;
    for (int c = 0; c < 26; ++c) {
        if (trie[0].next[c] != -1) {
            int v = trie[0].next[c];
            trie[v].fail = 0;
            q.push(v);
            if (log_flag) {
                log_file << "суффиксная ссылка вершины " << v << " -> 0 (корень)\n";
            }
        } else {
            trie[0].next[c] = 0;
        }
    }

    if (log_flag) {
        log_file << "\nпостроение суффиксных ссылок (обход в ширину):\n";
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int c = 0; c < 26; ++c) {
            int u = trie[v].next[c];
            if (u != -1) {
                trie[u].fail = trie[trie[v].fail].next[c];
                q.push(u);
                if (log_flag) {
                    log_file << "суффиксная ссылка вершины " << u << " -> " << trie[u].fail << "\n";
                }
            } else {
                trie[v].next[c] = trie[trie[v].fail].next[c];
            }
        }
    }

    if (log_flag) {
        log_file << "\nпостроение выходных ссылок:\n";
    }
    for (int v = 0; v < (int)trie.size(); ++v) {
        if (v == 0) {
            trie[v].out = -1;
            continue;
        }
        int f = trie[v].fail;
        if (trie[f].term) {
            trie[v].out = f;
        } else {
            trie[v].out = trie[f].out;
        }
        if (log_flag) {
            if (trie[v].out != -1) {
                log_file << "выходная ссылка вершины " << v << " -> " << trie[v].out << "\n";
            } else {
                log_file << "выходная ссылка вершины " << v << " отсутствует\n";
            }
        }
    }

    vector<int> memo_fail(trie.size(), -1);
    vector<int> memo_out(trie.size(), -1);
    int max_fail = 0;
    int max_out = 0;

    for (int v = 0; v < (int)trie.size(); ++v) {
        int flen = get_fail_len(v, trie, memo_fail);
        int olen = get_out_len(v, trie, memo_out);
        if (flen > max_fail) max_fail = flen;
        if (olen > max_out) max_out = olen;
    }

    if (log_flag) {
        log_file << "\nвычисление длин цепочек:\n";
        for (int v = 0; v < (int)trie.size(); ++v) {
            log_file << "вершина " << v << ": глубина по суффиксным ссылкам = " << memo_fail[v];
            log_file << ", глубина по выходным ссылкам = " << memo_out[v] << "\n";
        }
        log_file << "\nрезультат:\n";
        log_file << "длина самой длинной цепочки из суффиксных ссылок = " << max_fail << "\n";
        log_file << "длина самой длинной цепочки из конечных ссылок = " << max_out << "\n";
    }

    cout << max_fail << " " << max_out << endl;

    if (log_flag) {
        log_file.close();
    }

    return 0;
}