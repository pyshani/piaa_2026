#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <bitset>
#include <unistd.h>
#include <clocale>

#include <ctime>
#include <thread>

using namespace std;

int N;                         
int max_size;                   
int total_area;              
vector<unsigned long long> rows;     
vector<tuple<int,int,int>> best_solution;  
vector<tuple<int,int,int>> current;         
int best_count;                
bool graph_log;


void write_log(ofstream& file, const vector<tuple<int,int,int>>& squares) {
    vector<vector<string>> grid(N, vector<string>(N, "░"));

    for (size_t idx = 0; idx < squares.size(); ++idx) {
        char c;
        if (idx < 26)
            c = 'A' + idx;          
        else if (idx < 52)
            c = '1' + (idx - 26);   
        else
            c = '?';                 

        int x = get<0>(squares[idx]) - 1;
        int y = get<1>(squares[idx]) - 1;
        int s = get<2>(squares[idx]);

        for (int i = y; i < y + s; ++i)
            for (int j = x; j < x + s; ++j)
                grid[i][j] = c;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            file << grid[i][j] << " ";
        file << endl;
    }
    file << endl;
}


bool can_place(int x, int y, int s) {
    if (x + s > N || y + s > N) return false;
    unsigned long long mask = ((1ull << s) - 1) << x;
    for (int i = y; i < y + s; ++i)
        if (rows[i] & mask) return false;
    return true;
}


void place(int x, int y, int s, bool set, ofstream& file) {
    unsigned long long mask = ((1ull << s) - 1) << x;
    for (int i = y; i < y + s; ++i) {
        if (set) rows[i] |= mask;
        else rows[i] &= ~mask;
    }
    
    if (graph_log) {
        if (set) {
            file << "Установка квадрата в координаты " << "x,y = " << x << "," << y << " со стороной N = " << s << endl;
            write_log(file, current);
        } else {
            file << "Удаление квадрата в координатах " << "x,y = " << x << "," << y << " со стороной N = " << s << endl;
            write_log(file, current);
        }
    }
}


bool first_free(int &x, int &y) {
    for (y = 0; y < N; ++y) {
        if (rows[y] != (1ull << N) - 1) { 
            for (x = 0; x < N; ++x)
                if (!((rows[y] >> x) & 1)) return true;
        }
    }
    return false;
}


bool backtrack(int count, int area_used, int limit, ofstream& file) {
    
    if (count > limit) return false;

    int x, y;
    if (!first_free(x, y)) {         
        if (count < best_count) {
            best_count = count;
            best_solution = current;
        }
        return true;
    }

    int max_possible = min(N - x, N - y);
    max_possible = min(max_possible, max_size);
    
    for (int s = max_possible; s >= 1; --s) {
        if (count + 1 > limit) break;
        if (can_place(x, y, s)) {
            place(x, y, s, true, file);
            current.emplace_back(x + 1, y + 1, s);
            if (backtrack(count + 1, area_used + s * s, limit, file)) {
                place(x, y, s, false, file);
                current.pop_back();
                return true;
            }
            place(x, y, s, false, file);
            current.pop_back();
        }
    }
    return false;
}


bool easy_way(int N){
    if (N != 2 and N % 2 == 0) {
        int half = N / 2;

        cout << 4 << "\n";
        for (int j = 0; j < 2; j++) {
            int y = 1 + j * half;
            
            for (int i = 0; i < 2; i++) {
                int x = 1 + i * half;
                cout << x << " " << y << " " << half << "\n";
            }
        }
        return true;
    } if (N != 3 and N % 3 == 0){
        int third = N / 3;          
        int two_thirds = 2 * third;   
        cout << 6 << "\n";

        cout << "1 1 " << two_thirds << "\n";
        cout << two_thirds + 1 << " 1 " << third << "\n";
        cout << "1 " << two_thirds + 1 << " " << third << "\n";
        cout << two_thirds + 1 << " " << two_thirds + 1 << " " << third << "\n";
        cout << two_thirds + 1 << " " << 1 + third << " " << third << "\n";
        cout << 1 + third << " " << two_thirds + 1 << " " << third << "\n";

        return true;

    } if (N != 5 and N % 5 == 0){
        int fift = N / 5;
        int two_fift = 2 * fift;         
        int three_fift = 3 * fift;   
        cout << 8 << "\n";

        cout << "1 1 " << three_fift << "\n";

        cout << three_fift + 1 << " 1 " << two_fift << "\n";
        cout << "1 " << three_fift + 1 << " " << two_fift << "\n";

        cout << three_fift + 1 << " " << three_fift + 1 << " " << two_fift << "\n";
        
        cout << three_fift + 1 << " " << two_fift + 1 << " " << fift << "\n";
        cout << three_fift + 1 + fift << " " << two_fift + 1 << " " << fift << "\n";
        
        cout << two_fift + 1 << " " << three_fift + 1 << " " << fift << "\n";
        cout << two_fift + 1 << " " << three_fift + 1 + fift << " " << fift << "\n";

        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {

    ofstream outFile("output.log");
    ofstream graph_file("times.txt");

    graph_log = false;
    bool time_log = false;

    int opt;
    while ((opt = getopt(argc, argv, "lg")) != -1) {
        switch (opt) {
            case 'l':
                graph_log = true;
                break;
            case 'g':
                time_log = true;
                break;
            default:
                cerr << "Use: " << argv[0] << " [-l] [-g]" << endl;
                return 1;
        }
    }

    vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

    int time_cnt_iter = 1;
    if (time_log) time_cnt_iter = primes.size();
    else cin >> N;
    for(int i = 0; i < time_cnt_iter; i++){
        
        if (time_log) N = primes[i];
        auto start = std::chrono::high_resolution_clock::now();

        if(easy_way(N)) return 0;

        max_size = N - 1;
        total_area = N * N;
        best_count = total_area; 

        int first_size = int(max_size / 2) + 1;

        int squares_placed = 0;

        rows.assign(N, 0);   

        current.clear();
        
        current.emplace_back(1, 1, first_size);
        place(0, 0, first_size, true, outFile);
        
        current.emplace_back(first_size + 1, 1, first_size - 1);   
        place(first_size, 0, first_size - 1, true, outFile);

        current.emplace_back(1, first_size + 1, first_size - 1);
        place(0, first_size, first_size - 1, true, outFile);


        squares_placed += 3;

        int used_area = first_size * first_size + 2 * ((first_size - 1)*(first_size - 1));
        
        int lower = squares_placed + 1;
        

        for (int limit = lower; limit < best_count; ++limit) {
            if (backtrack(squares_placed, used_area, limit, outFile)) {
                break;                          
            }
        }

        if (best_count == total_area) {
            cout << total_area << "\n";
            for (int y = 0; y < N; ++y)
                for (int x = 0; x < N; ++x)
                    cout << x + 1 << " " << y + 1 << " 1\n";
        } else {
            cout << best_count << "\n";
            for (const auto& t : best_solution)
                cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << "\n";
        }

        outFile << "Итоговый квадрат:" << endl;
        write_log(outFile, best_solution);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
    
        graph_file << N << " " << duration.count() << endl;

        std::cout << "Elapsed time: " << duration.count() << " seconds." << std::endl;
    }

    return 0;
}