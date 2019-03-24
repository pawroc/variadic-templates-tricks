#include "print.hpp"
#include "tuple.hpp"
#include "print_template.hpp"
// #include "template_parameter_list.hpp"

#include <iostream>
#include <vector>
#include <map>

void test_print()
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
}

void test_tuple()
{
    tuple<double, uint64_t, const char*> t1(12.2, 42, "big");

    std::cout << "0th elem is " << get<0>(t1) << "\n";
    std::cout << "1st elem is " << get<1>(t1) << "\n";
    std::cout << "2nd elem is " << get<2>(t1) << "\n";
}

void test_print_template()
{
    std::vector<int> vec {1, 2, 3, 4, 5};
    std::map<int, std::string> m {{1, "one"}, {2, "two"}, {3, "three"}};

    print_container(vec);
    print_container(m);
}

int main()
{
    test_print();
    test_tuple();
    test_print_template();

    return 0;
}
