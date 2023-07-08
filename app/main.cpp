// Microbenchmarks for string comparison using Google benchmark
// Most of the code from:
// https://github.com/PacktPublishing/The-Art-of-Writing-Efficient-Programs
// Fedor G. Pikus
//
#include <cstdlib>
#include <cstring>
#include <memory>

#include "benchmark/benchmark.h"

using std::unique_ptr;

bool compare_int(const char* s1, const char* s2) {
    char c1, c2;
    for (int i1 = 0, i2 = 0; ; ++i1, ++i2) {
        c1 = s1[i1]; c2 = s2[i2];
        if (c1 != c2) return c1 > c2;
    }
}

bool compare_uint(const char* s1, const char* s2) {
    char c1, c2;
    for (unsigned int i1 = 0, i2 = 0; ; ++i1, ++i2) {
        c1 = s1[i1]; c2 = s2[i2];
        if (c1 != c2) return c1 > c2;
    }
}

bool compare_uint_l(const char* s1, const char* s2, unsigned int l) {
    if (s1 == s2) return false;
    char c1, c2;
    for (unsigned int i1 = 0, i2 = 0; i1 < l; ++i1, ++i2) {
        c1 = s1[i1]; c2 = s2[i2];
        if (c1 != c2) return c1 > c2;
    }
    return false;
}

void BM_loop_int(benchmark::State& state) {
    int64_t N = state.range(0);
    unique_ptr<char[]> s(new char[2 * N]);
    ::memset(s.get(), 'a', 2 * N * sizeof(char));
    s[2 * N - 1] = 0;
    const char* s1 = s.get();
    const char* s2 = s1 + N;
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare_int(s1, s2));
    }
    state.SetItemsProcessed(N * state.iterations());
}

void BM_loop_uint(benchmark::State& state) {
    int64_t N = state.range(0);
    unique_ptr<char[]> s(new char[2 * N]);
    ::memset(s.get(), 'a', 2 * N * sizeof(char));
    s[2 * N - 1] = 0;
    const char* s1 = s.get();
    const char* s2 = s1 + N;
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare_uint(s1, s2));
    }
    state.SetItemsProcessed(N * state.iterations());
}

void BM_loop_uint_l(benchmark::State& state) {
    int64_t N = state.range(0);
    unique_ptr<char[]> s(new char[2 * N]);
    ::memset(s.get(), 'a', 2 * N * sizeof(char));
    s[2 * N - 1] = 0;
    const char* s1 = s.get();
    const char* s2 = s1 + N;
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare_uint_l(s1, s2, static_cast<unsigned int>(2 * N)));
    }
    state.SetItemsProcessed(N * state.iterations());
}

#define ARGS \
    ->RangeMultiplier(2)->Range(1 << 10, 1 << 20)

BENCHMARK(BM_loop_int) ARGS;
BENCHMARK(BM_loop_uint) ARGS;
BENCHMARK(BM_loop_uint_l) ARGS;

BENCHMARK_MAIN();
