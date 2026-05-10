#include "geode.h"

static inline uint64_t rotl64(uint64_t x, int r) {
    return (x << r) | (x >> (64 - r));
}

void JavaRandom::setSeed(uint64_t value) {
    const uint64_t XL = 0x9e3779b97f4a7c15ULL;
    const uint64_t XH = 0x6a09e667f3bcc909ULL;
    const uint64_t A = 0xbf58476d1ce4e5b9ULL;
    const uint64_t B = 0x94d049bb133111ebULL;
    uint64_t l = value ^ XH;
    uint64_t h = l + XL;
    l = (l ^ (l >> 30)) * A;
    h = (h ^ (h >> 30)) * A;
    l = (l ^ (l >> 27)) * B;
    h = (h ^ (h >> 27)) * B;
    l = l ^ (l >> 31);
    h = h ^ (h >> 31);
    lo = l;
    hi = h;
}

uint64_t JavaRandom::nextLong() {
    uint64_t l = lo;
    uint64_t h = hi;
    uint64_t n = rotl64(l + h, 17) + l;
    h ^= l;
    lo = rotl64(l, 49) ^ h ^ (h << 21);
    hi = rotl64(h, 28);
    return n;
}

int JavaRandom::nextIntJ(uint32_t n) {
    int32_t bits;
    int32_t val;
    const int32_t m = (int32_t)n - 1;

    if ((m & (int32_t)n) == 0) {
        uint64_t x = n * (nextLong() >> 33);
        return (int)((int64_t)x >> 31);
    }

    do {
        bits = (int32_t)(nextLong() >> 33);
        val = bits % (int32_t)n;
    } while ((int32_t)((uint32_t)bits - val + m) < 0);

    return val;
}

float JavaRandom::nextFloat() {
    return (nextLong() >> (64 - 24)) * 5.9604645E-8F;
}

void JavaRandom::skipN(int count) {
    while (count-- > 0) {
        nextLong();
    }
}

uint64_t JavaRandom::nextLongJ() {
    int32_t a = nextLong() >> 32;
    int32_t b = nextLong() >> 32;
    return ((uint64_t)a << 32) + b;
}

static inline uint64_t getPopulationSeed(uint64_t ws, int x, int z) {
    JavaRandom xr;
    xr.setSeed(ws);
    uint64_t a = xr.nextLongJ();
    uint64_t b = xr.nextLongJ();
    a |= 1ULL;
    b |= 1ULL;
    return (static_cast<uint64_t>(x) * a + static_cast<uint64_t>(z) * b) ^ ws;
}

static inline optional<GeodeResult> findGeode(uint64_t worldSeed, int chunkX, int chunkZ) {
    const uint64_t salt = 20002ULL;
    const float rarity = 1.0f / 24.0f;
    int blockX = chunkX * 16;
    int blockZ = chunkZ * 16;
    int regionX = blockX & ~15;
    int regionZ = blockZ & ~15;

    JavaRandom xr;
    xr.setSeed(getPopulationSeed(worldSeed, regionX, regionZ) + salt);
    if (xr.nextFloat() >= rarity) {
        return nullopt;
    }

    int rx = xr.nextIntJ(16);
    int rz = xr.nextIntJ(16);
    rx -= blockX & 15;
    rz -= blockZ & 15;
    int ry = xr.nextIntJ(89) - 58;
    int size = xr.nextIntJ(2) + 3;
    xr.skipN(2);
    bool cracked = xr.nextFloat() < 0.95f;

    rx += 4;
    ry += 4;
    rz += 4;

    GeodeResult result;
    result.worldX = regionX + rx;
    result.worldY = ry;
    result.worldZ = regionZ + rz;
    result.size = size;
    result.cracked = cracked;
    return result;
}

bool isGeode(uint64_t ws, int cx, int cz) {
    const uint64_t salt = 20002ULL;
    const float rarity = 1.0f / 24.0f;
    int blockX = cx * 16;
    int blockZ = cz * 16;
    int regionX = blockX & ~15;
    int regionZ = blockZ & ~15;
    JavaRandom xr;
    xr.setSeed(getPopulationSeed(ws, regionX, regionZ) + salt);
    if (xr.nextFloat() >= rarity) {
        return false;
    }
    return true;
}