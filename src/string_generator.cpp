#include "string_generator.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

char StringGenerator::getRandomChar() {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(alphabet.size()) - 1);

    return alphabet[dist(rng)];
}

StringGenerator::StringGenerator(unsigned int seed) : rng(seed) {
}

std::size_t StringGenerator::getAlphabetSize() const {
    return alphabet.size();
}

std::string StringGenerator::generateRandomString() {
    std::uniform_int_distribution<int> lengthDist(MIN_STRING_LENGTH, MAX_STRING_LENGTH);

    int length = lengthDist(rng);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; i++) {
        result.push_back(getRandomChar());
    }

    return result;
}

std::vector<std::string> StringGenerator::generateRandomArray(int size) {
    std::vector<std::string> result;
    result.reserve(size);

    for (int i = 0; i < size; i++) {
        result.push_back(generateRandomString());
    }

    return result;
}

std::vector<std::string> StringGenerator::makeReverseSorted(std::vector<std::string> data) const {
    std::sort(data.begin(), data.end(), std::greater<std::string>());
    return data;
}

std::vector<std::string> StringGenerator::makeNearlySorted(std::vector<std::string> data, int swapsPerBlock) {
    std::sort(data.begin(), data.end());

    for (int blockStart = 0; blockStart < static_cast<int>(data.size()); blockStart += SIZE_STEP) {
        int blockEnd = std::min(blockStart + SIZE_STEP, static_cast<int>(data.size()));

        if (blockEnd - blockStart < 2) {
            continue;
        }

        std::uniform_int_distribution<int> indexDist(blockStart, blockEnd - 1);

        for (int j = 0; j < swapsPerBlock; j++) {
            int first = indexDist(rng);
            int second = indexDist(rng);

            if (first != second) {
                std::swap(data[first], data[second]);
            }
        }
    }

    return data;
}

std::vector<std::string> StringGenerator::generateCommonPrefixArray(int size, int prefixLength) {
    if (prefixLength < 0 || prefixLength >= MAX_STRING_LENGTH) {
        throw std::invalid_argument("Prefix length must be in range [0, 199].");
    }

    std::string prefix;
    prefix.reserve(prefixLength);

    for (int i = 0; i < prefixLength; i++) {
        prefix.push_back(getRandomChar());
    }

    std::vector<std::string> result;
    result.reserve(size);

    int minLength = std::max(MIN_STRING_LENGTH, prefixLength + 1);

    std::uniform_int_distribution<int> lengthDist(minLength, MAX_STRING_LENGTH);

    for (int i = 0; i < size; i++) {
        int totalLength = lengthDist(rng);

        std::string current = prefix;
        current.reserve(totalLength);

        while (static_cast<int>(current.size()) < totalLength) {
            current.push_back(getRandomChar());
        }

        result.push_back(current);
    }

    return result;
}

std::vector<std::string> StringGenerator::takeSubarray(const std::vector<std::string>& data, int size) const {
    if (size < 0 || size > static_cast<int>(data.size())) {
        throw std::invalid_argument("Invalid subarray size.");
    }

    return std::vector<std::string>(data.begin(), data.begin() + size);
}

void StringGenerator::saveArrayToFile(const std::vector<std::string>& data, const std::string& filename) const {
    std::ofstream fout(filename);

    if (!fout.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    for (const std::string& s : data) {
        fout << s << '\n';
    }
}

void StringGenerator::saveSubarraysToFolder(const std::vector<std::string>& maxArray, const std::string& folder,
                                            const std::string& filePrefix) const {
    fs::create_directories(folder);

    for (int size = SIZE_STEP; size <= MAX_ARRAY_SIZE; size += SIZE_STEP) {
        std::ostringstream filename;

        filename << folder << '/' << filePrefix << '_' << std::setw(4) << std::setfill('0') << size << ".txt";

        std::vector<std::string> subarray = takeSubarray(maxArray, size);
        saveArrayToFile(subarray, filename.str());
    }
}