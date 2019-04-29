#include <stdio.h>
#include <ctime>
#include <stdlib.h>

typedef unsigned long long llu;

int iAlignUp(int a, int b){
    return ((a % b) != 0) ?  (a - a % b + b) : a;
}

int iDivUp(int a, int b){
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
}

#define MT_RNG_COUNT 4096

const int    PATH_N = 24000000;
const int N_PER_RNG = iAlignUp(iDivUp(PATH_N, MT_RNG_COUNT), 2);
const int    RAND_N = MT_RNG_COUNT * N_PER_RNG;

typedef struct {
    llu s1, s2, s3, s4;
} tauswortheState;

llu lcg(llu s, llu a, llu b) {
    llu x = a * s + b;
    return x;
}

llu gen(tauswortheState *s) {
    llu b;
    b = (((s->s1 << 5) ^ s->s1) >> 39);
    s->s1 = (((s->s1 & 18446744073709551614ULL) << 24) ^ b);

    b = (((s->s2 << 19) ^ s->s2) >> 45);
    s->s2 = (((s->s2 & 18446744073709551552ULL) << 13) ^ b);

    b = (((s->s3 << 24) ^ s->s3) >> 48);
    s->s3 = (((s->s3 & 18446744073709551104ULL) << 7) ^ b);

    s->s4 = lcg(s->s4, 1664525, 1013904223ULL);

    return s->s1 ^ s->s2 ^ s->s3 ^ s->s4;
}

void initstate(tauswortheState *s, llu seed) {
    s->s1 = lcg(seed, 1664525, 1013904223ULL);
    if (s->s1 <= 128ULL) s->s1 += 128ULL;

    s->s2 = lcg(s->s1, 1664525, 1013904223ULL);
    if (s->s2 <= 128ULL) s->s2 += 128ULL;

    s->s3 = lcg(s->s2, 1664525, 1013904223ULL);
    if (s->s3 <= 128ULL) s->s3 += 128ULL;

    s->s4 = lcg(s->s3, 1664525, 1013904223ULL);
    if (s->s4 <= 128ULL) s->s4 += 128ULL;

    // Warm up
    gen(s);
    gen(s);
    gen(s);
}

int main() {
    llu seed = 1232;
    tauswortheState *state = (tauswortheState *)malloc(sizeof(tauswortheState));;
    
    const clock_t begin_time = clock();

    initstate(state, seed);
    llu randNum;
    for (int i = 0; i < RAND_N; ++i) {
        randNum = gen(state);
        // printf("%llu\n", randNum);
    }

    float runTime = (float)( clock() - begin_time ) /  CLOCKS_PER_SEC;
    printf("Time for generating %d random numbers on CPU: %fs\n\n", RAND_N, runTime);
}
