#include <cassert>
#include <cstdint>
#include <iostream>

// Таблица длинная, по этому посчитаем ее во время компиляции (С++14).
template<int N>
struct Table {
    constexpr Table() : t() {
        t[0] = 1;
        for (auto i = 1; i < N; ++i)
            t[i] = t[i - 1] * i; 
    }
    std::uint64_t t[N];
};

template<typename T>
T fac(T x) {
    static_assert(sizeof(T) <= sizeof(std::uint64_t), "T is too large");
    constexpr auto table = Table<66>();
    assert(x >= 0);
    return x < 66 ? static_cast<T>(table.t[x]) : 0;
}

int main() {    
    for (unsigned long long i = 0; i != 70; ++i) {
        std::cout << i << ": " << fac(i) << '\n';
    }
    std::cout << "the end\n";
}

