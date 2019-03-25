// https://jguegant.github.io/blogs/tech/sfinae-introduction.html#sfinae-introduction
// SFINAE stands for Substitution Failure Is Not An Error. In rough terms,
// a substitution is the mechanism that tries to replace the template parameters
// with the provided types or values. In some cases, if the substitution leads to
// an invalid code, the compiler shouldn't throw a massive amount of errors but
// simply continue to try the other available overloads.

#include <iostream>

struct Default {
    int foo() const {return 1;}
};

struct NonDefault {
    NonDefault(const NonDefault&) {}
    int foo() const {return 1;}
};

struct testStruct : std::true_type { }; // Inherit from the true type.

void test_basic_rules()
{
    decltype(Default().foo()) n1 = 1; // int n1
//  decltype(NonDefault().foo()) n2 = n1; // error: no default constructor
    decltype(std::declval<NonDefault>().foo()) n2 = n1; // int n2
    std::cout << "n2 = " << n2 << '\n';

    constexpr bool testVar = testStruct(); // Generate a compile-time testStruct.
    bool test = testStruct::value; // Equivalent to: test = true;
    test = testVar; // true_type has a constexpr converter operator, equivalent to: test = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

struct A {};

std::string to_string(const A&)
{
    return "I am a A!";
}

// Type B with a serialize method.
struct B
{
    std::string serialize() const
    {
        return "I am a B!";
    }
};

// Type C with a "wrong" serialize member (not a method) and a to_string overload.
struct C
{
    std::string serialize;
};

std::string to_string(const C&)
{
    return "I am a C!";
}

struct D : A
{
    std::string serialize() const
    {
        return "I am a D!";
    }
};

struct E
{
    struct Functor
    {
        std::string operator()(){
            return "I am a E!";
        }
    };

    Functor serialize;
};

template <class T> struct hasSerialize
{
    // We test if the type has serialize using decltype and declval.
    template <typename C> static constexpr decltype(std::declval<C>().serialize(), bool()) test(int /* unused */)
    {
        // We can return values, thanks to constexpr instead of playing with sizeof.
        return true;
    }

    template <typename C> static constexpr bool test(...)
    {
        return false;
    }

    // int is used to give the precedence!
    static constexpr bool value = test<T>(int());
};

template <typename T> typename std::enable_if<hasSerialize<T>::value, std::string>::type serialize1(T&& obj)
{
    return obj.serialize();
}

template <typename T> typename std::enable_if<!hasSerialize<T>::value, std::string>::type serialize1(T&& obj)
{
    return to_string(obj);
}

// Primary template, inherit from std::false_type.
// ::value will return false. 
// Note: the second unused template parameter is set to default as std::string!!!
template <typename T, typename = std::string>
struct hasSerialize2
        : std::false_type
{

};

// Partial template specialisation, inherit from std::true_type.
// ::value will return true. 
// The specialisation has therefore the precedence in the good cases. 
template <typename T>
struct hasSerialize2<T, decltype(std::declval<T>().serialize())>
        : std::true_type
{

};

template <typename T> typename std::enable_if<hasSerialize2<T>::value, std::string>::type serialize2(T&& obj)
{
    return obj.serialize();
}

template <typename T> typename std::enable_if<!hasSerialize2<T>::value, std::string>::type serialize2(T&& obj)
{
    return to_string(obj);
}


void test_has_serialized_1()
{
    A a;
    B b;
    C c;
    D d;
    E e;

    std::cout << serialize1(a) << '\n';
    std::cout << serialize1(b) << '\n';
    std::cout << serialize1(c) << '\n';
    std::cout << serialize1(d) << '\n';
    std::cout << serialize1(e) << '\n';
}

void test_has_serialized_2()
{
    A a;
    B b;
    C c;
    D d;
    E e;

    std::cout << serialize2(a) << '\n';
    std::cout << serialize2(b) << '\n';
    std::cout << serialize2(c) << '\n';
    std::cout << serialize2(d) << '\n';
    std::cout << serialize2(e) << '\n';
}

int main()
{
    test_basic_rules();
    test_has_serialized_1();
    test_has_serialized_2();
}