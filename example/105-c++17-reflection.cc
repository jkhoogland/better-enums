// This file was generated automatically.

// C++17 reflection proposal
//
// Better Enums can be used to implement the enums portion of the C++17
// reflection proposal N4428 in C++11. N4428 proposes the following traits
// interface:
//
// namespace std {
//
// template <typename E>
// struct enum_traits {
//     struct enumerators {
//         constexpr static size_t         size;
//
//         template <size_t I>
//         struct get {
//             constexpr string_literal    identifier;
//             constexpr static E          value;
//         };
//     };
// };
//
// }
//
// So, the basic usage would be:
//
// enum class Foo {A, B, C};
//
// constexpr size_t            size =
//     std::enum_traits<Foo>::enumerators::size;
//
// constexpr Foo               value_0 =
//     std::enum_traits<Foo>::enumerators::get<0>::value;
//
// constexpr string_literal    name_1 =
//     std::enum_traits<Foo>::enumerators::get<1>::identifier;
//
// Resulting in the values 3, Foo::A, and "B", respectively.

// The optional Better Enums header file extra/better-enums/n4428.h implements
// this interface, though with two necessary differences.
//
//   1. The interface is only available for Better Enums, i.e. enums declared
//      through the ENUM macro. This is because the macro is what generates the
//      information necessary for reflection.
//   2. The type of identifier is const char*, not the ${cxx17}-proposed
//      string_literal.
//
// Demo
//
// So, with that out of the way, we can do a little test. Let's assume that
// extra/ has been added as a directory to search for include files.

#ifndef BETTER_ENUMS_CONSTEXPR_TO_STRING
#define BETTER_ENUMS_CONSTEXPR_TO_STRING
#endif

#include <iostream>
#include <enum.h>
#include <better-enums/n4428.h>


// Let's declare an enum:

ENUM(Channel, char, Red = 1, Green, Blue)

// ...and try N4428:

constexpr std::size_t   size =
    std::enum_traits<Channel>::enumerators::size;

constexpr Channel       value_0 =
    std::enum_traits<Channel>::enumerators::get<0>::value;

constexpr Channel       value_1 =
    std::enum_traits<Channel>::enumerators::get<1>::value;

constexpr const char    *identifier_2 =
    std::enum_traits<Channel>::enumerators::get<2>::identifier;

// ...and check the results:

static_assert(size == 3, "");

static_assert(value_0 == +Channel::Red, "");
static_assert(value_1 == +Channel::Green, "");

int main()
{
    std::cout << identifier_2 << std::endl;
    return 0;
}

// That prints Blue, as you would expect.

// So, Better Enums can be used in C++11 to test N4428, and maybe start
// developing libraries on top of it. N4428 is very low-level, so it needs
// additional code over it to be truly useful. Whether developing now is a good
// idea is debatable, since N4428 is still only a proposal. But, it's an
// interesting thing to consider.
//
// Also, Better Enums can be implemented almost completely in terms of N4428, so
// the two interfaces are in some vaguely mathematical sense "equivalent." If
// N4428 is accepted, I may implement a variant of Better Enums on top of it,
// since it will remove many limitations.
//
// Quirk
//
// The reason for the #define in the code above is that there is one quirk: the
// interface above is available only for Better Enums for which compile-time
// name trimming is enabled, i.e. those declared when
// BETTER_ENUMS_CONSTEXPR_TO_STRING was defined, or declared with the SLOW_ENUM
// variant of ENUM. As mentioned on the linked page, the reason compile-time
// name trimming is not the default is that, while still pretty fast, it is four
// times slower than program-startup-time name trimming. The latter is the
// default.
//
// Despite the above, a variation on the interface is available for enums
// without compile-time name trimming:
//
// namespace std {
//
// template <typename E>
// struct enum_traits {
//     struct enumerators {
//         constexpr static size_t         size;
//
//         template <size_t I>
//         struct get {
//             constexpr const char        *identifier;
//             constexpr static E          value;
//         };
//
//         // For enums without compile-time name trimming.
//         template <size_t I>
//         struct get_alt {
//             static const char* identifier();
//             constexpr static E          value;
//         };
//     };
// };
//
// }
//
// As you can see, the difference is that identifier is a non-constexpr
// function, and you have to access it through get_alt<I>.
//
// // Without compile-time name trimming.
// ENUM(Depth, int, HighColor, TrueColor)
//
// int main()
// {
//     std::cout
//         << std::enum_traits<Depth>::enumerators::get_alt<1>::identifier()
//         << std::endl;
//
//     return 0;
// }
//
// The future
//
// N4428 is the fourth in a series of revisions: N3815, N4027, N4113, N4428. If
// there are more revisions that change the proposal for enums, I will try to
// implement those as well.
