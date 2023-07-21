#pragma once
#include <concepts>
#include <iostream>
#include <string>
#include <vector>

template<typename StreamType, typename T>
requires std::derived_from<StreamType, std::ostream>
auto serialize(StreamType& s, const T& t) -> void {
	s << t << ' ';
}

template<typename StreamType, typename T>
requires std::derived_from<StreamType, std::istream>
auto deserialize(StreamType& s, T& t) -> void {
	s >> t;
}

template<typename StreamType, typename T>
requires std::derived_from<StreamType,std::ostream>
    && std::same_as<std::string, T>
auto serialize(StreamType& s, const T& t) -> void {
    s << t.size() << " " << t;
}

template<typename StreamType, typename T>
requires std::derived_from<StreamType,std::istream>
	&& std::same_as<std::string, T>
auto deserialize(StreamType& s, T& t) -> void {
    t = "";
    typename T::size_type len;
    s >> len;

    for(std::size_t i = 0; i < len; ++i) {
        typename T::value_type c;
        s >> c;
        t += c;
    }
}

template<typename StreamType, typename T, typename Alloc>
requires std::derived_from<StreamType,std::ostream>
auto serialize(StreamType& s, const std::vector<T, Alloc>& t) -> void {
    s << t.size() << " ";
    for(const T& tt : t) {
        s << tt << " ";
    }
}

template<typename StreamType, typename T, typename Alloc>
requires std::derived_from<StreamType,std::istream>
auto deserialize(StreamType& s, std::vector<T, Alloc>& t) -> void {
    using VecType = std::vector<T, Alloc>;
    t.clear();

    typename VecType::size_type len;
    s >> len;
    for(std::size_t i = 0; i < len; ++i) {
        typename VecType::value_type c;
        s >> c;
        t.push_back(c);
    }
}

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define FOR_EACH_1(what, o, i, x)         \
    what(o, i, x)

#define FOR_EACH_2(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_1(what, o, i, __VA_ARGS__)

#define FOR_EACH_3(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_2(what, o, i, __VA_ARGS__)

#define FOR_EACH_4(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_3(what, o, i,  __VA_ARGS__)

#define FOR_EACH_5(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_4(what, o, i,  __VA_ARGS__)

#define FOR_EACH_6(what, x, ...)          \
  what(o, i, x);                          \
  FOR_EACH_5(what, o, i, __VA_ARGS__)

#define FOR_EACH_7(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_6(what, o, i,  __VA_ARGS__)

#define FOR_EACH_8(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_7(what, o, i,  __VA_ARGS__)

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, ...) CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__)
#define FOR_EACH(what, o, i, ...) \
FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, o, i, __VA_ARGS__)

#define MEMBER_SERIALIZE(outputStream, instance, memberName) \
        serialize(outputStream, instance.memberName);

#define MEMBER_DESERIALIZE(inputStream, instance, memberName) \
        deserialize(inputStream, instance.memberName);

#define SERIALIZE(className, ...)                                      \
friend auto operator<<(std::ostream& outputStream,                     \
        const className& instance) -> std::ostream& {                  \
    FOR_EACH(MEMBER_SERIALIZE, outputStream, instance, __VA_ARGS__)    \
    return outputStream;                                               \
}                                                                      \
                                                                       \
friend auto operator>>(std::istream& inputStream,                      \
        className& instance) -> std::istream& {                        \
    FOR_EACH(MEMBER_DESERIALIZE, inputStream, instance, __VA_ARGS__)   \
    return inputStream;                                                \
}
