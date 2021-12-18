#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <vector>

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
}


#endif // UTILS_H