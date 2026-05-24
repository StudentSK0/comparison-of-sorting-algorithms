#include "string_sort_algorithms.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

namespace {
const int ASCII_SIZE = 256;
const int SWITCH_THRESHOLD = 74;

int charAt(const string &s, int depth) {
  if (depth >= static_cast<int>(s.size())) {
    return -1;
  }

  return static_cast<unsigned char>(s[depth]);
}

int compareStrings(const string &a, const string &b,
                   long long &charComparisons) {
  int minLength = min(static_cast<int>(a.size()), static_cast<int>(b.size()));

  for (int i = 0; i < minLength; i++) {
    charComparisons++;

    if (a[i] < b[i]) {
      return -1;
    }

    if (a[i] > b[i]) {
      return 1;
    }
  }

  if (a.size() < b.size()) {
    return -1;
  }

  if (a.size() > b.size()) {
    return 1;
  }

  return 0;
}

void standardQuickSortRecursive(vector<string> &data, int left, int right,
                                long long &charComparisons) {
  if (left >= right) {
    return;
  }

  int i = left;
  int j = right;
  string pivot = data[left + (right - left) / 2];

  while (i <= j) {
    while (compareStrings(data[i], pivot, charComparisons) < 0) {
      i++;
    }

    while (compareStrings(data[j], pivot, charComparisons) > 0) {
      j--;
    }

    if (i <= j) {
      swap(data[i], data[j]);
      i++;
      j--;
    }
  }

  if (left < j) {
    standardQuickSortRecursive(data, left, j, charComparisons);
  }

  if (i < right) {
    standardQuickSortRecursive(data, i, right, charComparisons);
  }
}

void standardMergeSortRecursive(vector<string> &data, vector<string> &buffer,
                                int left, int right,
                                long long &charComparisons) {
  if (right - left <= 1) {
    return;
  }

  int middle = left + (right - left) / 2;

  standardMergeSortRecursive(data, buffer, left, middle, charComparisons);
  standardMergeSortRecursive(data, buffer, middle, right, charComparisons);

  int i = left;
  int j = middle;
  int k = left;

  while (i < middle && j < right) {
    if (compareStrings(data[i], data[j], charComparisons) <= 0) {
      buffer[k] = data[i];
      i++;
    } else {
      buffer[k] = data[j];
      j++;
    }

    k++;
  }

  while (i < middle) {
    buffer[k] = data[i];
    i++;
    k++;
  }

  while (j < right) {
    buffer[k] = data[j];
    j++;
    k++;
  }

  for (int index = left; index < right; index++) {
    data[index] = buffer[index];
  }
}

void ternaryStringQuickSortRecursive(vector<string> &data, int left, int right,
                                     int depth, long long &charComparisons) {
  if (left >= right) {
    return;
  }

  int pivotIndex = left + (right - left) / 2;
  int pivot = charAt(data[pivotIndex], depth);

  int lt = left;
  int gt = right;
  int i = left;

  while (i <= gt) {
    int current = charAt(data[i], depth);

    charComparisons++;
    if (current < pivot) {
      swap(data[lt], data[i]);
      lt++;
      i++;
      continue;
    }

    charComparisons++;
    if (current > pivot) {
      swap(data[i], data[gt]);
      gt--;
      continue;
    }

    i++;
  }

  ternaryStringQuickSortRecursive(data, left, lt - 1, depth, charComparisons);

  if (pivot != -1) {
    ternaryStringQuickSortRecursive(data, lt, gt, depth + 1, charComparisons);
  }

  ternaryStringQuickSortRecursive(data, gt + 1, right, depth, charComparisons);
}

struct LcpCompareResult {
  int cmp;
  int lcp;
};

LcpCompareResult lcpCompare(const string &a, const string &b, int start,
                            long long &charComparisons) {
  int i = start;

  while (i < static_cast<int>(a.size()) && i < static_cast<int>(b.size())) {
    charComparisons++;

    if (a[i] != b[i]) {
      break;
    }

    i++;
  }

  int lcp = i;

  if (i == static_cast<int>(a.size()) && i == static_cast<int>(b.size())) {
    return {0, lcp};
  }

  if (i == static_cast<int>(a.size())) {
    return {-1, lcp};
  }

  if (i == static_cast<int>(b.size())) {
    return {1, lcp};
  }

  if (a[i] < b[i]) {
    return {-1, lcp};
  }

  return {1, lcp};
}

struct LcpRun {
  vector<string> values;

  /*
      lcpPrev[i] = LCP(values[i - 1], values[i]).
      Для i = 0 значение равно 0.
  */
  vector<int> lcpPrev;
};

LcpRun mergeLcpRuns(const LcpRun &left, const LcpRun &right,
                    long long &charComparisons) {
  LcpRun result;

  int leftSize = static_cast<int>(left.values.size());
  int rightSize = static_cast<int>(right.values.size());

  result.values.reserve(leftSize + rightSize);
  result.lcpPrev.reserve(leftSize + rightSize);

  int i = 0;
  int j = 0;

  bool hasLast = false;

  int hLeft = 0;
  int hRight = 0;

  auto appendValue = [&](const string &value, int lcpWithLast) {
    result.values.push_back(value);

    if (!hasLast) {
      result.lcpPrev.push_back(0);
      hasLast = true;
    } else {
      result.lcpPrev.push_back(lcpWithLast);
    }
  };

  while (i < leftSize && j < rightSize) {
    if (!hasLast) {
      LcpCompareResult comparison =
          lcpCompare(left.values[i], right.values[j], 0, charComparisons);

      if (comparison.cmp <= 0) {
        appendValue(left.values[i], 0);
        i++;

        if (i < leftSize) {
          hLeft = left.lcpPrev[i];
        }

        if (j < rightSize) {
          hRight = comparison.lcp;
        }
      } else {
        appendValue(right.values[j], 0);
        j++;

        if (i < leftSize) {
          hLeft = comparison.lcp;
        }

        if (j < rightSize) {
          hRight = right.lcpPrev[j];
        }
      }

      continue;
    }

    bool takeLeft;
    int lcpBetweenCurrentStrings;

    if (hLeft > hRight) {
      takeLeft = true;
      lcpBetweenCurrentStrings = hRight;
    } else if (hLeft < hRight) {
      takeLeft = false;
      lcpBetweenCurrentStrings = hLeft;
    } else {
      LcpCompareResult comparison =
          lcpCompare(left.values[i], right.values[j], hLeft, charComparisons);

      takeLeft = comparison.cmp <= 0;
      lcpBetweenCurrentStrings = comparison.lcp;
    }

    if (takeLeft) {
      appendValue(left.values[i], hLeft);
      i++;

      if (j < rightSize) {
        hRight = lcpBetweenCurrentStrings;
      }

      if (i < leftSize) {
        hLeft = left.lcpPrev[i];
      }
    } else {
      appendValue(right.values[j], hRight);
      j++;

      if (i < leftSize) {
        hLeft = lcpBetweenCurrentStrings;
      }

      if (j < rightSize) {
        hRight = right.lcpPrev[j];
      }
    }
  }

  while (i < leftSize) {
    appendValue(left.values[i], hasLast ? hLeft : 0);
    i++;

    if (i < leftSize) {
      hLeft = left.lcpPrev[i];
    }
  }

  while (j < rightSize) {
    appendValue(right.values[j], hasLast ? hRight : 0);
    j++;

    if (j < rightSize) {
      hRight = right.lcpPrev[j];
    }
  }

  return result;
}

LcpRun lcpMergeSortRecursive(const vector<string> &data, int left, int right,
                             long long &charComparisons) {
  if (right - left == 1) {
    LcpRun run;
    run.values.push_back(data[left]);
    run.lcpPrev.push_back(0);
    return run;
  }

  int middle = left + (right - left) / 2;

  LcpRun leftRun = lcpMergeSortRecursive(data, left, middle, charComparisons);
  LcpRun rightRun = lcpMergeSortRecursive(data, middle, right, charComparisons);

  return mergeLcpRuns(leftRun, rightRun, charComparisons);
}

void msdRadixSortRecursive(vector<string> &data, int left, int right, int depth,
                           long long &charComparisons) {
  if (left >= right) {
    return;
  }

  vector<vector<string>> buckets(ASCII_SIZE + 1);

  for (int i = left; i <= right; i++) {
    int c = charAt(data[i], depth);

    /*
        Для MSD radix sort это не сравнение двух символов,
        а посимвольная операция чтения/классификации.
    */
    charComparisons++;

    buckets[c + 1].push_back(data[i]);
  }

  int position = left;

  for (int bucketIndex = 0; bucketIndex <= ASCII_SIZE; bucketIndex++) {
    int bucketStart = position;

    for (const string &value : buckets[bucketIndex]) {
      data[position] = value;
      position++;
    }

    int bucketEnd = position - 1;

    if (bucketIndex > 0 && bucketStart < bucketEnd) {
      msdRadixSortRecursive(data, bucketStart, bucketEnd, depth + 1,
                            charComparisons);
    }
  }
}

void msdRadixSortWithQuickSortRecursive(vector<string> &data, int left,
                                        int right, int depth,
                                        long long &charComparisons) {
  if (left >= right) {
    return;
  }

  int fragmentSize = right - left + 1;

  if (fragmentSize < SWITCH_THRESHOLD) {
    ternaryStringQuickSortRecursive(data, left, right, depth, charComparisons);
    return;
  }

  vector<vector<string>> buckets(ASCII_SIZE + 1);

  for (int i = left; i <= right; i++) {
    int c = charAt(data[i], depth);
    charComparisons++;
    buckets[c + 1].push_back(data[i]);
  }

  int position = left;

  for (int bucketIndex = 0; bucketIndex <= ASCII_SIZE; bucketIndex++) {
    int bucketStart = position;

    for (const string &value : buckets[bucketIndex]) {
      data[position] = value;
      position++;
    }

    int bucketEnd = position - 1;

    if (bucketIndex > 0 && bucketStart < bucketEnd) {
      msdRadixSortWithQuickSortRecursive(data, bucketStart, bucketEnd,
                                         depth + 1, charComparisons);
    }
  }
}
} // namespace

void StringSortAlgorithms::standardQuickSort(vector<string> &data,
                                             long long &charComparisons) {
  if (data.empty()) {
    return;
  }

  standardQuickSortRecursive(data, 0, static_cast<int>(data.size()) - 1,
                             charComparisons);
}

void StringSortAlgorithms::standardMergeSort(vector<string> &data,
                                             long long &charComparisons) {
  if (data.empty()) {
    return;
  }

  vector<string> buffer(data.size());

  standardMergeSortRecursive(data, buffer, 0, static_cast<int>(data.size()),
                             charComparisons);
}

void StringSortAlgorithms::ternaryStringQuickSort(vector<string> &data,
                                                  long long &charComparisons) {
  if (data.empty()) {
    return;
  }

  ternaryStringQuickSortRecursive(data, 0, static_cast<int>(data.size()) - 1, 0,
                                  charComparisons);
}

void StringSortAlgorithms::lcpMergeSort(vector<string> &data,
                                        long long &charComparisons) {
  if (data.empty()) {
    return;
  }

  LcpRun sortedRun = lcpMergeSortRecursive(
      data, 0, static_cast<int>(data.size()), charComparisons);

  data = sortedRun.values;
}

void StringSortAlgorithms::msdRadixSort(vector<string> &data,
                                        long long &charComparisons) {
  if (data.empty()) {
    return;
  }

  msdRadixSortRecursive(data, 0, static_cast<int>(data.size()) - 1, 0,
                        charComparisons);
}

void StringSortAlgorithms::msdRadixSortWithQuickSort(
    vector<string> &data, long long &charComparisons) {
  if (data.empty()) {
    return;
  }

  msdRadixSortWithQuickSortRecursive(data, 0, static_cast<int>(data.size()) - 1,
                                     0, charComparisons);
}
