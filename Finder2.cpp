#include <bits/stdc++.h>
#include "geode.h"
using namespace std;
const uint64_t rangeBounds[2][2] = {
    {19,19},{20,20}
};
bool inrange(int val, int l, int r){
    return val >= l && val <= r;
}
int main() {
    auto start = chrono::high_resolution_clock::now();
    for (uint64_t seed = 0; seed < 10000000000; seed++) {
        bool bad = false;
        uint64_t popseeds[2][2]{};
        for (int cx = 0; cx <= 1; cx++) {
            for (int cz = 0; cz <= 0; cz++) {
                int bx = cx * 16;
                int bz = cz * 16;
                int rx = bx & ~15;
                int rz = bz & ~15;
                uint64_t popseed = getPopulationSeed(seed, rx, rz);
                popseeds[cx][cz] = popseed;
                if (!isGeode(seed, cx, cz, popseed)) {
                    bad = true;
                    break;
                }
            }
            if (bad) {
                break;
            }
        }
        if (!bad) {
            bool valid = true;
            int firsty = -65;
            for (int cx = 0; cx <= 1; cx++) {
                for (int cz = 0; cz <= 0; cz++) {
                    auto geode = findGeode(seed, cx, cz, popseeds[cx][cz]);
                    int x = geode->worldX;
                    int z = geode->worldZ;
                    int y = geode->worldY;
                    if (inrange(x, rangeBounds[cx][0], rangeBounds[cx][1]) 
                    && inrange(z, rangeBounds[cz][0], rangeBounds[cz][1])) {
                        if(firsty == -65) {
                            firsty = y;
                        } else if (!inrange(y, firsty, firsty)) {
                            valid = false;
                            break;
                        }
                    }
                    else {
                        valid = false;
                        break;
                    }
                    if (!valid) {
                        break;
                    }
                }
                if (!valid) {
                    break;
                }
            }
            if (valid) {
                cout << "Seed: " << seed << endl;
            }
        }
        if (seed % 100000000 == 0) {
            cout << "Checked " << seed << " seeds";
            auto now = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::seconds>(now - start).count();
            cout << " Time elapsed: " << duration << " seconds" << endl;
        }
    }
}