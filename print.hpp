#include <iostream>
#include <vector>
#include <sstream>

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

template<typename ... T>
std::vector<std::string> print3(const T& ... t)
{
    std::vector<std::string> retval;
    std::stringstream ss;

    (void)std::initializer_list<int>
    {
        // it is guaranteed that all functions will be called in order
        (
            ss.str(""),
            ss << t,
            retval.push_back(ss.str()),
            0
        )...
    };
    return retval;
}


// What is `(print_impl(t), 0)`? It means:
// execute f1(), then execute f2() and return last value - it is ret val from f2()
