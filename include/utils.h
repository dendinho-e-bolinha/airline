#ifndef AIRLINE_UTILS_H
#define AIRLINE_UTILS_H

#include <functional>
#include <vector>
#include <algorithm>
#include "interact.h"

namespace utils {

    /**
     * @brief Returns an iterator pointing to the first element in the vector that is not less than (i.e. greater or equal to) value, or last if no such element is found. 
     * @param vec Partially-Ordered vector to examine
     * @param value Value to compare the elements to 
     * @param mapper binary predicate which returns ​true if the first argument is less than (i.e. is ordered before) the second. 
     *
     * @return Iterator pointing to the first element that is not less than key. If no such element is found, a past-the-end iterator (see end()) is returned. 
     */
    template <typename T, typename U>
    typename std::vector<T>::iterator lowerBound(std::vector<T> &vec, const U &value, const std::function<U(T)> &mapper) {
        using Iterator = typename std::vector<T>::iterator;
        auto first = vec.begin();
        auto last = vec.end();
        
        Iterator it;
        typename std::iterator_traits<Iterator>::difference_type count, step;
        count = std::distance(first, last);
    
        while (count > 0) {
            it = first; 
            step = count / 2; 
            std::advance(it, step);

            if (mapper(*it) < value) {
                first = ++it; 
                count -= step + 1; 
            }
            else
                count = step;
        }

        return first;
    }

    /**
     * @brief Checks if an element equivalent to value appears within the vector.
     * @param vec Ordered vector to examine
     * @param value Value to compare the elements to 
     * @param mapper Binary predicate which returns ​true if the first argument is less than (i.e. is ordered before) the second. 
     * @return 
     */
    template <typename T, typename U>
    T binarySearch(std::vector<T> &vector, const U &value, const std::function<U(T)> &mapper) {
        auto pos = lowerBound<T, U>(vector, value, mapper);

        if (pos != vector.end() && !(value < mapper(*pos)))
            return *pos;

        return nullptr;  
    }

    /**
     * @brief Sorts the elements in the vector into ascending order
     * @param pool Vector to be sorted
     * @param mapper Binary function that accepts two elements in the range as arguments, and returns a value convertible to bool. The value returned indicates whether the element passed as first argument is considered to go before the second in the specific strict weak ordering it defines.
     */
    template <typename T, typename V>
    void sort(std::vector<T> &pool, const std::function<V(const T&)> &mapper) {
        std::stable_sort(pool.begin(), pool.end(), [&mapper](const T &t1, const T &t2) {
            return mapper(t1) < mapper(t2);
        });
    }

    /**
     * @brief Reverses the elements order in the vector
     * @param pool Vector to be reversed
     */
    template <typename T>
    void reverse(std::vector<T> &pool) {
        auto left = pool.begin();
        auto right = --pool.end();

        while (left < right) {
            T temp = *left;
            *left = *right;
            *right = temp;

            left++;
            right--;
        }
    }

    /**
     * @brief Slices the vector from the start containing a number of elements
     * specified by the user
     * @param pool Vector to be sliced
     */
    template <typename T>
    void sliceVectorFromBeginWithUserInput(std::vector<T> &pool) {
        std::size_t number = readValue<std::size_t>("#: ", "Please provide a valid number of elements");
        auto last = pool.begin() + number - 1;
        for (auto it = pool.end() - 1; it > last ; it--)
            pool.erase(it);
    }

    /**
     * @brief Slices the vector from a vector element until the end containing 
     * a number of elements specified by the user
     * @param pool Vector to be sliced
     */
    template <typename T>
    void sliceVectorFromEndWithUserInput(std::vector<T> &pool) {
        std::size_t number = readValue<std::size_t>("#: ", "Please provide a valid number of elements");
        auto first = pool.end() - number;
        for (auto it = pool.begin(), cursor = first; it < first; it++, cursor++)
            *it = *cursor;

        pool.resize(number);
    }
}


#endif // UTILS_H