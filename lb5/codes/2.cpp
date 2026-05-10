#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

const int ALPHABET_SIZE = 5;

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

struct Node {
    Node* son[ALPHABET_SIZE];
    Node* go[ALPHABET_SIZE];
    Node* parent;
    Node* suffLink;
    Node* up;
    char charToParent;
    bool isLeaf;
    vector<int> leafPatternNumber;

    Node() {
        memset(son, 0, sizeof(son));
        memset(go, 0, sizeof(go));
        parent = nullptr;
        suffLink = nullptr;
        up = nullptr;
        isLeaf = false;
    }
};

Node* root;

Node* getLink(Node* v, char c);
Node* getUp(Node* v);


Node* getSuffLink(Node* v) {
    if (v->suffLink == nullptr) {
        if (v == root || v->parent == root) {
            v->suffLink = root;
        } else {
            v->suffLink = getLink(getSuffLink(v->parent), v->charToParent);
        }
    }
    return v->suffLink;
}


Node* getLink(Node* v, char c) {
    int idx = char_to_idx(c);
    if (idx == -1) {
        cout << "Error: '" << c <<  "' symbol not in alfabet" << endl;
        return 0;
    }
    if (v->go[idx] == nullptr) {
        if (v->son[idx]) {
            v->go[idx] = v->son[idx];
        } else if (v == root) {
            v->go[idx] = root;
        } else {
            v->go[idx] = getLink(getSuffLink(v), c);
        }
    }
    return v->go[idx];
}


Node* getUp(Node* v) {
    if (v->up == nullptr) {
        Node* suff = getSuffLink(v);
        if (suff->isLeaf) {
            v->up = suff;
        } else if (suff == root) {
            v->up = root;
        } else {
            v->up = getUp(suff);
        }
    }
    return v->up;
}


void addString(const string& s, int patternNumber) {
    Node* cur = root;
    for (size_t i = 0; i < s.length(); ++i) {
        char c = s[i];
        int idx = char_to_idx(c);
        if (idx == -1) {
            cout << "Error: '" << c <<  "' symbol not in alfabet" << endl;
            exit(0);
        }
        if (cur->son[idx] == nullptr) {
            cur->son[idx] = new Node();
            cur->son[idx]->parent = cur;
            cur->son[idx]->charToParent = c;
        }
        cur = cur->son[idx];
    }
    cur->isLeaf = true;
    cur->leafPatternNumber.push_back(patternNumber);
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool logflag = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--logflag") == 0) {
            logflag = true;
            break;
        }
    }

    ofstream logfile;
    if (logflag) {
        logfile.open("log2.txt");
        if (!logfile) {
            cerr << "error: cannot open log2.txt for writing" << endl;
            return 1;
        }
        logfile << "Отладочный вывод алгоритма Ахо-Корасик (ленивая рекурсия)" << endl;
    }

    string T, P, wc_str;
    cin >> T >> P >> wc_str;
    char joker = wc_str[0];

    if (logfile.is_open()) {
        logfile << "текст T = " << T << endl;
        logfile << "шаблон P = " << P << endl;
        logfile << "joker = " << joker << endl;
    }

    vector<int> pat_len;
    vector<int> pat_offset;
    int m = (int)P.size();

    for (int i = 0; i < m; ++i) {
        if (P[i] != joker) {
            int j = i;
            while (j < m && P[j] != joker) ++j;
            pat_offset.push_back(i);
            pat_len.push_back(j - i);
            i = j - 1;
        }
    }

    int k = (int)pat_len.size();
    if (logfile.is_open()) {
        logfile << "найдено сегментов: " << k << endl;
        for (int i = 0; i < k; ++i) {
            logfile << "  сегмент " << i << ": offset=" << pat_offset[i]
                    << ", длина=" << pat_len[i] << ", текст=\""
                    << P.substr(pat_offset[i], pat_len[i]) << "\"" << endl;
        }
    }

    root = new Node();
    root->parent = root;
    root->suffLink = root;
    root->up = root;

    for (int idx = 0; idx < k; ++idx) {
        string segment = P.substr(pat_offset[idx], pat_len[idx]);
        addString(segment, idx);
        if (logfile.is_open()) {
            logfile << "сегмент " << idx << " добавлен в бор" << endl;
        }
    }

    vector<int> cnt(T.size() + 5, 0);
    int n = (int)T.size();

    Node* cur = root;
    for (int j = 0; j < n; ++j) {
        char c = T[j];
        int idx = char_to_idx(c);
        if (idx == -1) {
            cout << "Error: '" << c <<  "' symbol not in alfabet" << endl;
            return 0;
        }
        Node* prev = cur;
        cur = getLink(cur, c);  

        if (logfile.is_open()) {
            logfile << "  позиция " << j + 1 << " символ '" << c
                    << "' -> переход " << prev << " -> " << cur << endl;
        }

        Node* tmp = cur;
        while (tmp != root) {
            if (tmp->isLeaf) {
                for (int segmentIdx : tmp->leafPatternNumber) {
                    int start_frag = j - pat_len[segmentIdx] + 1;
                    int start_candidate = start_frag - pat_offset[segmentIdx];
                    if (start_candidate >= 0 && start_candidate + m <= n) {
                        cnt[start_candidate]++;
                        if (logfile.is_open()) {
                            logfile << "    найден сегмент " << segmentIdx
                                    << " (длина " << pat_len[segmentIdx]
                                    << ", offset " << pat_offset[segmentIdx]
                                    << ") -> позиция в тексте " << start_candidate + 1
                                    << " (cnt=" << cnt[start_candidate] << ")" << endl;
                        }
                    }
                }
            }
            tmp = getUp(tmp);
        }
    }

    if (logfile.is_open()) {
        logfile << "итоговые совпадения (начальные позиции в 1-индексации):" << endl;
    }

    for (int i = 0; i + m <= n; ++i) {
        if (cnt[i] == k) {
            cout << i + 1 << '\n';
            if (logfile.is_open()) {
                logfile << "  " << i + 1 << endl;
            }
        }
    }

    if (logfile.is_open()) {
        logfile << "отладка завершена." << endl;
        logfile.close();
    }

    return 0;
}