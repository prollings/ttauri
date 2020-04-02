// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "TTauri/Foundation/required.hpp"
#include <algorithm>
#include <tuple>

namespace TTauri {

/** Transform an input container to the output container.
 * @param input Input container.
 * @param operation A function to execute on each element in the input
 * @return Output container containing the transformed elements.
 */
template<typename T, typename U, typename F>
inline T transform(const U &input, F operation)
{
    T result = {};
    result.reserve(input.size());
    std::transform(input.begin(), input.end(), std::back_inserter(result), operation);
    return result;
}

/** Generate data in an array.
 * @param operation A function to execute for each element in the array.
 *                  The function accepts a single index argument.
 * @return An array filled with elements generated by operation.
 */
template<typename T, size_t N, typename F>
constexpr std::array<T, N> generate_array(F operation)
{
    std::array<T, N> a{};

    for (size_t i = 0; i < N; i++) {
        a.at(i) = operation(i);
    }

    return a;
}

/** Remove an element from an container.
 * @param v Container with elements that need to be removed.
 * @param predicate A function that is passed to std::find_if() to find elements that need to be erased.
 */
template<typename T, typename F>
inline void erase_if(T &v, F predicate)
{
    while (true) {
        let i = std::find_if(v.begin(), v.end(), predicate);
        if (i == v.end()) {
            return;
        }
        v.erase(i);
    }
}


template<typename It, typename UnaryPredicate>
constexpr It rfind_if(It const first, It const last, UnaryPredicate predicate)
{
    auto i = last;
    do {
        i--;
        if (predicate(*i)) {
            return i;
        }
    } while (i != first);
    return last;
}

template<typename It, typename UnaryPredicate>
constexpr It rfind_if_not(It const first, It const last, UnaryPredicate predicate)
{
    return rfind_if(first, last, [&](auto const &x) { return !predicate(x); });
}

template<typename It, typename T>
constexpr It rfind(It const first, It const last, T const &value)
{
    return rfind_if(first, last, [&](auto const &x) { return x == value; });
}

/** Find the start of the current cluster.
* @param last The last iterator, where this function will stop iterating.
* @param start Where to start the search
* @param predicate A function returning the identifier of the cluster.
* @return One beyond the last iterator where the cluster is the same as start.
*/
template<typename ConstIt, typename It, typename UnaryPredicate>
constexpr It find_cluster(ConstIt last, It start, UnaryPredicate predicate)
{
    let cluster_id = predicate(*start);

    for (auto i = start + 1; i != last; ++i) {
        if (predicate(*i) != cluster_id) {
            return i;
        }
    }
    return last;
}

/** Find the start of the current cluster.
 * @param first The first iterator, where this function will stop iterating.
 * @param start Where to start the search
 * @param predicate A function returning the identifier of the cluster.
 * @return The first iterator where the cluster is the same as start.
 */
template<typename ConstIt, typename It, typename UnaryPredicate>
constexpr It rfind_cluster(ConstIt first, It start, UnaryPredicate predicate)
{
    let cluster_id = predicate(*start);

    if (start == first) {
        return first;
    }

    auto i = start - 1;
    while (true) {
        if (predicate(*i) != cluster_id) {
            return (i + 1);
        }

        if (i == first) {
            return i;
        }
        --i;
    }
    ttauri_unreachable();
}

/** Find the begin and end of the current cluster.
* @param first The first iterator, where this function will stop iterating.
* @param first The last iterator, where this function will stop iterating.
* @param start Where to start the search
* @param predicate A function returning the identifier of the cluster.
* @return The first and one beyond last iterator where the cluster is the same as start.
*/
template<typename ConstIt, typename It, typename UnaryPredicate>
constexpr std::pair<It,It> bifind_cluster(ConstIt first, ConstIt last, It start, UnaryPredicate predicate)
{
    return {
        rfind_cluster(first, start, predicate),
        find_cluster(last, start, predicate)
    };
}

/*! For each cluster.
 * func() is executed for each cluster that is found between first-last.
 * A cluster is found between two separators, a separator is detected with IsClusterSeperator().
 * A cluster does not include the separator itself.
 */
template<typename It, typename S, typename F>
inline void for_each_cluster(It first, It last, S IsClusterSeperator, F Function)
{
    if (first == last) {
        return;
    }

    // If the first item is a cluster seperator skip over it.
    if (IsClusterSeperator(*first)) {
        first++;
    }

    for (auto i = first; i != last;) {
        auto j = std::find_if(i, last, IsClusterSeperator);
        Function(i, j);

        auto skipOverSeperator = (j == last) ? 0 : 1;
        i = j + skipOverSeperator;
    }
}

template<typename InputIt1, typename InputIt2>
inline bool starts_with(InputIt1 haystack_first, InputIt1 haystack_last, InputIt2 needle_first, InputIt2 needle_last) noexcept
{
    let [haystack_result, needle_result] = std::mismatch(haystack_first, haystack_last, needle_first, needle_last);
    return needle_result == needle_last;
}

template<typename Container1, typename Container2>
inline bool starts_with(Container1 haystack, Container2 needle) noexcept
{
    return starts_with(haystack.begin(), haystack.end(), needle.begin(), needle.end());
}

template<typename InputIt1, typename InputIt2, typename BinaryPredicate>
inline std::pair<InputIt1,InputIt2> rmismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate predicate) noexcept
{
    auto i1 = last1;
    auto i2 = last2;

    while (true) {
        if (i1 == first1 && i2 == first2) {
            return {last1, last2};
        } else if (i1 == first1) {
            return {last1, --i2};
        } else if (i2 == first2) {
            return {--i1, last2};
        }

        if (!predicate(*(--i1), *(--i2))) {
            return {i1, i2};
        }
    }
}

template<typename InputIt1, typename InputIt2>
inline std::pair<InputIt1,InputIt2> rmismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) noexcept
{
    return rmismatch(first1, last1, first2, last2, [&](auto a, auto b) { return a == b; });  
}

template<typename InputIt1, typename InputIt2>
inline bool ends_with(InputIt1 haystack_first, InputIt1 haystack_last, InputIt2 needle_first, InputIt2 needle_last) noexcept
{
    let [haystack_result, needle_result] = rmismatch(haystack_first, haystack_last, needle_first, needle_last);
    return needle_result == needle_last;
}

template<typename Container1, typename Container2>
inline bool ends_with(Container1 haystack, Container2 needle) noexcept
{
    return ends_with(haystack.begin(), haystack.end(), needle.begin(), needle.end());
}

}

