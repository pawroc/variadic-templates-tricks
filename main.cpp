#include "print.hpp"

#include <iostream>

int main()
{
    std::cout << "#### print1 ###\n";
    print1("Hello", "World", 1, 2.4);
    
    std::cout << "\n#### print2 ###\n";
    print2("Hello", "World", 1, 2.4);
    
    std::cout << "\n#### print3 ###\n";
    for (const auto& v : print3("Hello", "World", 1, 2.4))
    {
        std::cout << v << "\n";
    }

    std::cout << "\n#### (f1(), f2()) ###\n";
    int ret = (f1(), f2());
    std::cout << "Ret: " << ret << "\n";

    return 0;
}
