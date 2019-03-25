// https://jguegant.github.io/blogs/tech/sfinae-introduction.html#sfinae-introduction
// SFINAE stands for Substitution Failure Is Not An Error. In rough terms,
// a substitution is the mechanism that tries to replace the template parameters
// with the provided types or values. In some cases, if the substitution leads to
// an invalid code, the compiler shouldn't throw a massive amount of errors but
// simply continue to try the other available overloads.

#include <iostream>

/*
 The compiler will try this overload since it's less generic than the variadic.
 T will be replace by int which gives us void f(const int& t, int::iterator* b = nullptr);
 int doesn't have an iterator sub-type, but the compiler doesn't throw a bunch of errors.
 It simply tries the next overload. 
*/
template <typename T> void f(const T& t, typename T::iterator* it = nullptr)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

// The sink-hole.
void f(...)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

///////////////////////////////////////////////////////////////

// Compile time comparisons
typedef char yes;  // Size: 1 byte
typedef yes no[2]; // Size 2 bytes

yes& f1();
no& f2();

///////////////////////////////////////////////////////////////

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

// Structures are good as they expose everything in public!
template <class T> struct hasSerialize
{
    // For the compile time comparison.
    typedef char yes[1];
    typedef yes no[2];

    // This helper struct permits us to check two properties of a template argument.
    template <typename U, U u> struct reallyHas;

    // Two overloads for yes: one if for the signature of a normal method, one is for the signature of a const method.
    template <typename C> static yes& test(reallyHas<std::string (C::*)(), &C::serialize>* /*unused*/) { }
    template <typename C> static yes& test(reallyHas<std::string (C::*)() const, &C::serialize>* /*unused*/) { }

    // The famous C++ sink-hole.
    // Note that sink-hole must be templated too as we are testing test<T>(0).
    template <typename> static no& test(...) { /* dark matter */ }

    // The constant used as a return value for the test.
    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

template <class T> bool testHasSerialize(const T& /*t*/) { return hasSerialize<T>::value; }

template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

template <class T> typename enable_if<hasSerialize<T>::value, std::string>::type serialize(const T& obj)
{
    return obj.serialize();
}

template <class T> typename enable_if<!hasSerialize<T>::value, std::string>::type serialize(const T& obj)
{
    return to_string(obj);
}

int main()
{
    f(1); // Calls void f(...) { }

    std::cout << (sizeof(f1()) == sizeof(f2())) << std::endl; // Output 0.
    std::cout << (sizeof(f1()) == sizeof(f1())) << std::endl; // Output 1.

    // Using the struct A, B, C defined in the previous hasSerialize example.
    A a;
    B b;
    C c;

    std::cout << serialize(a) << std::endl;
    std::cout << serialize(b) << std::endl;
    std::cout << serialize(c) << std::endl;
}