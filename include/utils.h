#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <vector>
#include <algorithm>

namespace utils {

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

    template <typename T, typename U>
    T binarySearch(std::vector<T> &vector, const U &value, const std::function<U(T)> &mapper) {
        auto pos = lowerBound<T, U>(vector, value, mapper);

        if (pos != vector.end() && !(value < mapper(*pos)))
            return *pos;

        return nullptr;  
    }

    template<typename T, typename V>
    void sort(std::vector<T> &pool, const std::function<V(const T&)> &mapper) {
        std::stable_sort(pool.begin(), pool.end(), [&mapper](const T &t1, const T &t2) {
            return mapper(t1) < mapper(t2);
        });
    }

    template<typename T>
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
}


#endif // UTILS_H