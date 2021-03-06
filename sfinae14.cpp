// https://jguegant.github.io/blogs/tech/sfinae-introduction.html#sfinae-introduction
// SFINAE stands for Substitution Failure Is Not An Error. In rough terms,
// a substitution is the mechanism that tries to replace the template parameters
// with the provided types or values. In some cases, if the substitution leads to
// an invalid code, the compiler shouldn't throw a massive amount of errors but
// simply continue to try the other available overloads.

#include <iostream>
#include <type_traits>

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
        std::string operator()() {
            return "I am a E!";
        }
    };

    Functor serialize;
};



template <typename UnnamedType> struct container
{
// Let's put the test in private.
private:
    // We use std::declval to 'recreate' an object of 'UnnamedType'.
    // We use std::declval to also 'recreate' an object of type 'Param'.
    // We can use both of these recreated objects to test the validity!
    template <typename... Params> constexpr auto test_validity(int /* unused */)
    -> decltype(std::declval<UnnamedType>()(std::declval<Params>()...), std::true_type())
    {
        // If substitution didn't fail, we can return a true_type.
        return std::true_type();
    }

    template <typename... Params> constexpr std::false_type test_validity(...)
    {
        // Our sink-hole returns a false_type.
        return std::false_type();
    }

public:
    // A public operator() that accept the argument we wish to test onto the UnnamedType.
    // Notice that the return type is automatic!
    template <typename... Params> constexpr auto operator()(Params&& ...)
    {
        // The argument is forwarded to one of the two overloads.
        // The SFINAE on the 'true_type' will come into play to dispatch.
        return test_validity<Params...>(int());
    }
};

template <typename UnnamedType> constexpr auto is_valid(UnnamedType&& t)
{
    return container<UnnamedType>();
}

// Check if a type has a serialize method.
auto hasSerialize = is_valid([](auto&& x) -> decltype(x.serialize()) { });

template <class T> auto serialize(T& obj) -> typename std::enable_if<decltype(hasSerialize(obj))::value, std::string>::type
{
    return obj.serialize();
}

template <class T> auto serialize(T& obj) -> typename std::enable_if<!decltype(hasSerialize(obj))::value, std::string>::type
{
    return to_string(obj);
}

int main() {
    A a;
    B b;
    C c;
    D d;
    E e;

    std::cout << serialize(a) << std::endl;
    std::cout << serialize(b) << std::endl;
    std::cout << serialize(c) << std::endl;
    std::cout << serialize(d) << std::endl;
    std::cout << serialize(e) << std::endl;

    return 0;
}
