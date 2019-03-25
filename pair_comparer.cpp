#include <iostream>

template<typename T, typename U>
bool pair_comparer(T a, U b) {
    // In real-world code, we wouldn't compare floating point values like
    // this. It would make sense to specialize this function for floating
    // point types to use approximate comparison.
    std::cout << "1) Compare: " << a << " and " << b << "\n";
    return a == b;
}

template<typename T>
bool pair_comparer(T a) {
    return false;
}

template<typename T, typename U, typename... Args>
bool pair_comparer(T a, U b, Args... args) {
    std::cout << "2) Compare: " << a << " and " << b << "\n";
    return a == b && pair_comparer(args...);
}

int main()
{
    if (pair_comparer(10, 10, 11.1f, 11.1f/*, "aaa", "aaa", 100, 100.0 */))
    {
        std::cout << "Equal!\n";
    }
    else
    {
        std::cout << "Not equal!\n";
    }
    return 0;
}