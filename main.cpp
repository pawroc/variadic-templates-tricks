#include <iostream>

template<typename T>
void print_impl(const T& t)
{
    std::cout << t << '\n';
}

template<typename ... T>
void print1(const T& ... t)
{
    (void)std::initializer_list<int>{ (print_impl(t), 0) ... };
}

void print2()
{
    return;
}

template<typename T, typename ... U>
void print2(const T& t, const U& ... u)
{
    print_impl(t);
    print2(u...);
}

int f1()
{
    std::cout << "f1\n";
    return 1;
}

int f2()
{
    std::cout << "f2\n";
    return 2;
}

// What is `(print_impl(t), 0)`? It means:
// execute f1(), then execute f2() and return last value - it is ret val from f2()

int main()
{
    std::cout << "#### print1 ###\n";
    print1("Hello", "World", 1, 2.4);
    
    std::cout << "\n#### print2 ###\n";
    print2("Hello", "World", 1, 2.4);
    
    std::cout << "\n#### (f1(), f2()) ###\n";
    int ret = (f1(), f2());
    std::cout << "Ret: " << ret << "\n";

    return 0;
}
