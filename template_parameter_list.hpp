#include <iostream>

template<typename... T> struct value_holder
{
    value_holder()
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        // apply<T...> a(&T...);
    }

    template<T... Values> // expands to a non-type template parameter 
    struct apply          // list, such as <int, char, int(&)[5]>
    {
        apply() { std::cout << __PRETTY_FUNCTION__ << "\n"; }
    };
};