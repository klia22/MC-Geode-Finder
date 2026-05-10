#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <atomic>
#include "geode.h"
using namespace std;

mutex cout_mutex;
atomic<long long> total_checked(0);
atomic<long long> last_reported(0);

void search_seeds(long long start, long long end, int thread_id) {
    for(long long seed = start; seed < end; seed++) {
        int bad = 2;   
        for(int cx = -1; cx <= 1; cx++) {
            for(int cz = -1; cz <= 1; cz++) {
                if(bad <= 0 || !isGeode(seed, cx, cz)) {
                    bad--;
                    if (bad <= 0) {
                        break;
                    }
                }
            }
            if(bad <= 0) {
                break;
            }
        }
        if(bad > 0) {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Seed: " << seed << endl;
        }
        total_checked++;
        long long current = total_checked.load();
        if (current % 100000000 == 0 && current / 100000000 > last_reported.load()) {
            long long report_val = current / 100000000;
            if (last_reported.load() < report_val) {
                last_reported.store(report_val);
                lock_guard<mutex> lock(cout_mutex);
                cout << "Progress: " << current << " seeds checked" << endl;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int num_threads = thread::hardware_concurrency();
    if (argc > 1) {
        num_threads = stoi(argv[1]);
    }
    
    cout << "Using " << num_threads << " threads\n";
    
    auto start = chrono::high_resolution_clock::now();
    long long total_seeds = 100000000000LL;
    long long seeds_per_thread = total_seeds / num_threads;
    
    vector<thread> threads;
    for(int i = 0; i < num_threads; i++) {
        long long thread_start = i * seeds_per_thread;
        long long thread_end = (i == num_threads - 1) ? total_seeds : (i + 1) * seeds_per_thread;
        threads.emplace_back(search_seeds, thread_start, thread_end, i);
    }
    
    for(auto& t : threads) {
        t.join();
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    cout << "Time: " << elapsed.count() << " ms\n";
    cout << "Total seeds checked: " << total_checked << "\n";
}
