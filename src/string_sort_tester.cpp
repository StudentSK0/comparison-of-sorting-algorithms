#include "string_sort_tester.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

bool StringSortTester::isSortedLexicographically(const vector<string>& data) {
    for (int i = 1; i < static_cast<int>(data.size()); i++) {
        if (data[i - 1] > data[i]) {
            return false;
        }
    }

    return true;
}

TestResult StringSortTester::runAlgorithm(const vector<string>& source, const string& algorithmName,
                                          const string& dataType, int arraySize, int runId,
                                          const function<void(vector<string>&, long long&)>& sortFunction) const {
    vector<string> data = source;
    long long charComparisons = 0;

    auto start = chrono::high_resolution_clock::now();

    sortFunction(data, charComparisons);

    auto end = chrono::high_resolution_clock::now();

    if (!isSortedLexicographically(data)) {
        throw runtime_error(algorithmName + " produced incorrect result.");
    }

    long long timeMicroseconds = chrono::duration_cast<chrono::microseconds>(end - start).count();

    return {algorithmName, dataType, arraySize, runId, timeMicroseconds, charComparisons};
}

void StringSortTester::writeRawResults(const vector<TestResult>& results, const string& filename) {
    ofstream fout(filename);

    fout << "algorithm,data_type,array_size,run_id,time_us,char_comparisons\n";

    for (const TestResult& result : results) {
        fout << result.algorithm << ',' << result.dataType << ',' << result.arraySize << ',' << result.runId << ','
             << result.timeMicroseconds << ',' << result.charComparisons << '\n';
    }
}

void StringSortTester::writeAverageResults(const vector<TestResult>& rawResults, const string& filename) {
    ofstream fout(filename);

    fout << "algorithm,data_type,array_size,avg_time_us,avg_char_comparisons\n";
    fout << fixed << setprecision(2);

    if (rawResults.empty()) {
        return;
    }

    for (int i = 0; i < static_cast<int>(rawResults.size());) {
        string algorithm = rawResults[i].algorithm;
        string dataType = rawResults[i].dataType;
        int arraySize = rawResults[i].arraySize;

        long long totalTime = 0;
        long long totalComparisons = 0;
        int count = 0;

        int j = i;

        while (j < static_cast<int>(rawResults.size()) && rawResults[j].algorithm == algorithm &&
               rawResults[j].dataType == dataType && rawResults[j].arraySize == arraySize) {
            totalTime += rawResults[j].timeMicroseconds;
            totalComparisons += rawResults[j].charComparisons;
            count++;
            j++;
        }

        fout << algorithm << ',' << dataType << ',' << arraySize << ',' << static_cast<double>(totalTime) / count << ','
             << static_cast<double>(totalComparisons) / count << '\n';

        i = j;
    }
}