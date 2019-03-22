#include "print.hpp"
#include "tuple.hpp"
// #include "template_parameter_list.hpp"

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

    // value_holder<int, char, int> holder;
    // auto t = holder.apply;
    tuple<double, uint64_t, const char*> t1(12.2, 42, "big");

    std::cout << "0th elem is " << get<0>(t1) << "\n";
    std::cout << "1st elem is " << get<1>(t1) << "\n";
    std::cout << "2nd elem is " << get<2>(t1) << "\n";

    return 0;
}
