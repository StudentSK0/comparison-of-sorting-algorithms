#ifndef STRING_SORT_ALGORITHMS_H
#define STRING_SORT_ALGORITHMS_H

#include <string>
#include <vector>

class StringSortAlgorithms {
   public:
    static void standardQuickSort(std::vector<std::string>& data, long long& charComparisons);

    static void standardMergeSort(std::vector<std::string>& data, long long& charComparisons);

    static void ternaryStringQuickSort(std::vector<std::string>& data, long long& charComparisons);

    static void lcpMergeSort(std::vector<std::string>& data, long long& charComparisons);

    static void msdRadixSort(std::vector<std::string>& data, long long& charComparisons);

    static void msdRadixSortWithQuickSort(std::vector<std::string>& data, long long& charComparisons);
};

#endif