#include <bits/stdc++.h>
#include <optional>
using namespace std;
#ifndef GEODE_H
#define GEODE_H
static inline uint64_t rotl64(uint64_t x, int r);

struct JavaRandom {
    uint64_t lo;
    uint64_t hi;

    void setSeed(uint64_t value);

    uint64_t nextLong();

    int nextIntJ(uint32_t n);

    float nextFloat();

    void skipN(int count);

    uint64_t nextLongJ();
};

static inline uint64_t getPopulationSeed(uint64_t ws, int x, int z);

struct GeodeResult {
    int worldX = 0;
    int worldY = 0;
    int worldZ = 0;
    int size = 0;
    bool cracked = false;
};

static inline optional<GeodeResult> findGeode(uint64_t worldSeed, int chunkX, int chunkZ);

bool isGeode(uint64_t ws, int cx, int cz);
#endif