#include <random>
#include <vector>
#include <algorithm>

#ifdef STRICT_RAND

struct RandomDevice {
  std::random_device                    rd;
  std::mt19937_64                       int_gen;
  std::uniform_real_distribution<float> dis_real;
  std::uniform_int_distribution<int>    dis_int;

  RandomDevice() :
    int_gen(rd()) {}

  inline void  srand(int seed) {}
  inline int   rand() { return dis_int(int_gen); }
  inline float frand() { return dis_real(int_gen); }
};
#else


struct RandomDevice {
  unsigned int g_seed;

  RandomDevice() { g_seed = 0; }

  // Used to seed the generator.
  inline void srand(int seed) {
    g_seed = seed;
  }

  // Compute a pseudorandom integer.
  // Output value in range [0, 32767]
  inline int rand() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
  }


  inline float frand() { return rand() / float(32767); }
};

#endif

struct ExperimentStats {
  int partialEvenChoiceHigher = 0;
  int partialEvenChioceLower  = 0;
  int partialGreater          = 0;
  int partialLower            = 0;
  int partialGreaterSecond    = 0;
  int partialLowerSecond      = 0;

  void print(int fd) {
    dprintf(2, "partialEvenChoiceHigher = %d\n", partialEvenChoiceHigher);
    dprintf(2, "partialEvenChoiceLower = %d\n", partialEvenChioceLower);
    dprintf(2, "partialGreater = %d\n", partialGreater);
    dprintf(2, "partialLower = %d\n", partialLower);
    dprintf(2, "partialGreaterSecond = %d\n", partialGreaterSecond);
    dprintf(2, "partialLowerSecond = %d\n", partialLowerSecond);
  }
};

inline ExperimentStats stats;

struct BinQueries {
  RandomDevice* device;
  int           medianBinLoad;
  int           medianMedianBinLoad;

  void update(std::vector<int> v) {
    std::sort(v.begin(), v.end());
    medianMedianBinLoad = v[v.size() / 4];
    medianBinLoad       = v[v.size() / 2];
  }

  int pickRandom(int a, int b) {
    if (device->frand() < 0.5) return a;
    return b;
  }
  int pickBins(int b, int b2, int l, int l2, int k) {
    if (k < 1) return pickRandom(b, b2);
    if (l > medianBinLoad && l2 > medianBinLoad) {
      stats.partialEvenChoiceHigher++;
      return pickRandom(b, b2);
    }

    if (l > medianBinLoad && l2 < medianBinLoad) {
      stats.partialGreater++;
      return b2;
    }
    if (l < medianBinLoad && l2 > medianBinLoad) {
      stats.partialLower++;
      return b;
    }

    if (l < medianBinLoad && l2 < medianMedianBinLoad) {
      if (k < 2) return pickRandom(b, b2);

      if (l > medianMedianBinLoad && l2 > medianMedianBinLoad) {
        stats.partialEvenChioceLower++;
        return pickRandom(b, b2);
      }

      if (l > medianMedianBinLoad && l2 < medianMedianBinLoad) {
        stats.partialGreaterSecond++;
        return b2;
      }
      if (l < medianMedianBinLoad && l2 > medianMedianBinLoad) {
        stats.partialLowerSecond++;
        return b;
      }
    }

    stats.partialEvenChioceLower++;
    return pickRandom(b, b2);
  }
};
