#include "string_generator.h"
#include "string_sort_algorithms.h"
#include "string_sort_tester.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

int main() {
    constexpr int MAX_ARRAY_SIZE = 3000;
    constexpr int SIZE_STEP = 100;
    constexpr int RUNS = 10;

    fs::create_directories("results");

    StringGenerator generator(42);
    StringSortTester tester;

    vector<string> randomData = generator.generateRandomArray(MAX_ARRAY_SIZE);

    vector<string> reverseSortedData = generator.makeReverseSorted(randomData);

    vector<string> nearlySortedData = generator.makeNearlySorted(randomData);

    vector<string> commonPrefixData = generator.generateCommonPrefixArray(MAX_ARRAY_SIZE, 60);

    vector<pair<string, vector<string>>> datasets = {{"random", randomData},
                                                     {"reverse_sorted", reverseSortedData},
                                                     {"nearly_sorted", nearlySortedData},
                                                     {"common_prefix", commonPrefixData}};

    vector<AlgorithmConfig> algorithms = {
        {"standard_quicksort", StringSortAlgorithms::standardQuickSort},
        {"standard_mergesort", StringSortAlgorithms::standardMergeSort},
        {"ternary_string_quicksort", StringSortAlgorithms::ternaryStringQuickSort},
        {"lcp_mergesort", StringSortAlgorithms::lcpMergeSort},
        {"msd_radix_sort", StringSortAlgorithms::msdRadixSort},
        {"msd_radix_sort_with_quicksort", StringSortAlgorithms::msdRadixSortWithQuickSort}};

    vector<TestResult> rawResults;

    for (const auto& dataset : datasets) {
        const string& dataType = dataset.first;
        const vector<string>& maxArray = dataset.second;

        cout << "Dataset: " << dataType << '\n';

        for (int size = SIZE_STEP; size <= MAX_ARRAY_SIZE; size += SIZE_STEP) {
            vector<string> source = generator.takeSubarray(maxArray, size);

            cout << "  Size: " << size << '\n';

            for (const AlgorithmConfig& algorithm : algorithms) {
                cout << "    Algorithm: " << algorithm.name << '\n';

                for (int runId = 1; runId <= RUNS; runId++) {
                    rawResults.push_back(
                        tester.runAlgorithm(source, algorithm.name, dataType, size, runId, algorithm.sortFunction));
                }
            }
        }
    }

    StringSortTester::writeRawResults(rawResults, "results/sort_results_raw.csv");

    StringSortTester::writeAverageResults(rawResults, "results/sort_results_avg.csv");

    cout << "Measurements completed.\n";
    cout << "Raw results: results/sort_results_raw.csv\n";
    cout << "Average results: results/sort_results_avg.csv\n";

    return 0;
}