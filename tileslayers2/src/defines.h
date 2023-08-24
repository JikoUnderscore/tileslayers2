#pragma once

#define fn [[nodiscard]] auto
#define static_fn [[nodiscard]] static auto
#define let const auto
#define bg_internal static

#if _MSC_VER < 1300
using u8 = unsigned char;
using i8 = signed char;
using u16 = unsigned short;
using i16 = signed short;
using u32 = unsigned int;
using i32 = signed int;

#else
using u8 = unsigned __int8;
using i8 = signed __int8;
using u16 = unsigned __int16;
using i16 = signed __int16;
using u32 = unsigned __int32;
using i32 = signed __int32;
using u64 = unsigned __int64;
using i64 = signed __int64;
#endif

using f32 = float;
using f64 = double;

// #define DEL_COPY_MOVE(TypeName) \
//     TypeName(TypeName const& other) = delete; \
//     void operator=(const TypeName&) = delete; \
//     TypeName(TypeName&& other) noexcept = delete; \
//     TypeName& operator=(TypeName&& other) noexcept = delete


// #ifndef _DEBUG //  VS only
// #define DEBUG(...)
// #else

// #include <iostream>
// template <typename... Ts>
// void DEBUG(Ts&&... ts) {
//     ((std::cerr << std::forward<Ts>(ts)), ...) << std::endl;
// }
// #endif

#ifdef _MSC_VER
using usize = unsigned __int64;
using isize = __int64;
#endif

#ifdef __GNUC__
using usize = __SIZE_TYPE__;
using isize = __PTRDIFF_TYPE__;
#endif

#ifdef __clang__
using usize = __SIZE_TYPE__;
using isize = __PTRDIFF_TYPE__;
#endif


////////////////////////////////////////////////////////////////
//
// Defer statement
// Akin to D's SCOPE_EXIT or
// similar to Go's defer but scope-based
//
// NOTE: C++11 (and above) only!
//
// NOTE(bill): Stupid fucking templates
template <typename T>
struct gbRemoveReference {
    using Type = T;
};
template <typename T>
struct gbRemoveReference<T&> {
    using Type = T;
};
template <typename T>
struct gbRemoveReference<T&&> {
    using Type = T;
};

/// NOTE(bill): `"Move`" semantics - invented because the C++ committee are idiots (as a collective not as indiviuals (well a least some aren't))
template <typename T>
inline auto gb_forward(typename gbRemoveReference<T>::Type& t) -> T&& {
    return static_cast<T&&>(t);
}
template <typename T>
inline auto gb_forward(typename gbRemoveReference<T>::Type&& t) -> T&& {
    return static_cast<T&&>(t);
}
// template <typename T>
// inline auto gb_move(T&& t) -> T&& {
//     return static_cast<typename gbRemoveReference<T>::Type&&>(t);
// }

template <typename F>
struct gbprivDefer {
    F f;
    explicit gbprivDefer(F&& f) : f(gb_forward<F>(f)) {
    }
    ~gbprivDefer() {
        f();
    }
};

template <typename F>
auto gb__defer_func(F&& f) -> gbprivDefer<F> {
    return gbprivDefer<F>(gb_forward<F>(f));
}

#define GB_DEFER_1(x, y) x##y
#define GB_DEFER_2(x, y) GB_DEFER_1(x, y)
#define GB_DEFER_3(x) GB_DEFER_2(x, __COUNTER__)
#define defer(code) auto GB_DEFER_3(_defer_) = gb__defer_func([&]() -> void { (code); })
