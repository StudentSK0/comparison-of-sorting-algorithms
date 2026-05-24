#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <functional>
#include <string>
#include <vector>

struct TestResult {
    std::string algorithm;
    std::string dataType;
    int arraySize;
    int runId;
    long long timeMicroseconds;
    long long charComparisons;
};

struct AlgorithmConfig {
    std::string name;
    std::function<void(std::vector<std::string>&, long long&)> sortFunction;
};

class StringSortTester {
   public:
    TestResult runAlgorithm(const std::vector<std::string>& source, const std::string& algorithmName,
                            const std::string& dataType, int arraySize, int runId,
                            const std::function<void(std::vector<std::string>&, long long&)>& sortFunction) const;

    static void writeRawResults(const std::vector<TestResult>& results, const std::string& filename);

    static void writeAverageResults(const std::vector<TestResult>& rawResults, const std::string& filename);

   private:
    static bool isSortedLexicographically(const std::vector<std::string>& data);
};

#endif