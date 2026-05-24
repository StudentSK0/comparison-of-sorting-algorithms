#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <random>
#include <string>
#include <vector>

class StringGenerator {
   private:
    static constexpr int MIN_STRING_LENGTH = 10;
    static constexpr int MAX_STRING_LENGTH = 200;
    static constexpr int MAX_ARRAY_SIZE = 3000;
    static constexpr int SIZE_STEP = 100;

    const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#%:;^&*()-.";

    std::mt19937 rng;

    char getRandomChar();

   public:
    explicit StringGenerator(unsigned int seed = 42);

    std::size_t getAlphabetSize() const;

    std::string generateRandomString();

    std::vector<std::string> generateRandomArray(int size = MAX_ARRAY_SIZE);

    std::vector<std::string> makeReverseSorted(std::vector<std::string> data) const;

    std::vector<std::string> makeNearlySorted(std::vector<std::string> data, int swapsPerBlock = 2);

    std::vector<std::string> generateCommonPrefixArray(int size = MAX_ARRAY_SIZE, int prefixLength = 60);

    std::vector<std::string> takeSubarray(const std::vector<std::string>& data, int size) const;

    void saveArrayToFile(const std::vector<std::string>& data, const std::string& filename) const;

    void saveSubarraysToFolder(const std::vector<std::string>& maxArray, const std::string& folder,
                               const std::string& filePrefix) const;
};

#endif