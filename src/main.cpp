#include "config.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <vector>
#include "algorithm.hpp"

using namespace std;

RandomDevice randomDevice;

inline bool prob(float f) {
  return f > randomDevice.frand();
}

/* Computes the maximum load of the bins*/
float getLoad(std::vector<int>& bins) {
  int maximumLoad = 0;

  for (int i = 0; i < bins.size(); i++) {
    maximumLoad = std::max(maximumLoad, bins[i]);
  }
  return maximumLoad;
}

/* Computes the gap of the bins for a given number of balls*/
float getGap(std::vector<int>& bins, int n) {

  float maximumGap = -numeric_limits<float>::infinity();
  float s          = n / (float)bins.size();

  for (int i = 0; i < bins.size(); i++) {
    maximumGap = std::max(maximumGap, bins[i] - s);
  }
  return maximumGap;
}

/* Assuming we are only doing a random selection of a maximum of 3 bins in our
 * set */
using BinSelection = std::array<int, 32>;

/* Pick d bins uniformly from a set of m bins */
void pickBins(int d, int m, BinSelection& selection) {

  for (int i = 0; i < d; i++) {
    int  candidate;
    bool skip;
    do {
      candidate = randomDevice.rand() % m;
      skip      = true;
      for (int j = 0; j < i; j++) {
        if (candidate == selection[j]) {
          skip = false;
          break;
        }
      }

    } while (!skip);

    selection[i] = candidate;
  }
}

/* Return the lightest bin of the selection */
int pickLightestBin(std::vector<int>& bins, const BinSelection& indices, int d) {
  int index        = indices[0];
  int lighest_load = bins[indices[0]];

  for (int i = 1; i < d; i++) {
    int j    = indices[i];
    int load = bins[j];
    if (load < lighest_load) {
      lighest_load = load;
      index        = j;
    }
  }
  return index;
}

int pickBinFromPartialInformation(std::vector<int>& bins, BinQueries queries, const BinSelection& selection, int d, const ExperimentConfiguration& config) {
  int selected = selection[0];
  if (d == 3) {
    int a = queries.pickBins(selection[0], selection[1], bins[selection[0]], bins[selection[1]], config.k);
    int b = queries.pickBins(selection[0], selection[2], bins[selection[0]], bins[selection[2]], config.k);

    if (a != b) {
      selected = queries.pickBins(a, b, bins[a], bins[b], config.k);
    } else {
      selected = a;
    }
  } else if (d == 2) {
    selected = queries.pickBins(selection[0], selection[1], bins[selection[0]], bins[selection[1]], config.k);
  } else {
    selected = selection[0];
  }
  return selected;
}

/* Perform allocation of batches of b size algorithm to the set of bins using d choice */
/* Single allocation is just allocation of batches of size 1 */
void allocate(std::vector<int>& bins, int n, int b, const ExperimentConfiguration& config) {

  for (int i = 0; i < n; i++) {

    int d = config.d + (config.useBetaProbability && prob(config.betaProbability));

    BinSelection selection;
    pickBins(d, bins.size(), selection);

    if (config.partialInformation) {
      BinQueries queries;
      queries.device = &randomDevice;
      queries.update(bins);

      int selected = pickBinFromPartialInformation(bins, queries, selection, d, config);
      bins[selected] += b;

    } else {
      int selected = pickLightestBin(bins, selection, d);
      bins[selected] += b;
    }
  }
}

/* prints allocation of bins for test purposes */
void printAllocation(std::vector<int>& bins) {
  for (int i = 0; i < bins.size(); i++) {
    cout << bins[i] << "\t";
  }
  cout << endl;
}

/* Computes the single allocation trial for given size of bin set and d choices
 * and prints the result information.
 * */

void singleExperiment(ExperimentConfiguration configuration) {
  int m          = configuration.m;
  int iterations = configuration.iterations;

  vector<int> bins(m);
  vector<int> lowerGaps(m);
  vector<int> higherGaps(m);

  auto computeGap = [&](int n, int i, vector<int>& gaps) {
    // Clear bins
    for (int j = 0; j < m; j++)
      bins[j] = 0;

    int bsize;

    if (configuration.useSingleAllocation) {
      allocate(bins, n, bsize = 1, configuration);
    } else {
      allocate(bins, n, bsize = configuration.m, configuration);
    }

    if (configuration.outputLoad) {
      gaps[i] += getLoad(bins);

    } else {
      // Get gap reasults for given m,d,n configuration
      gaps[i] += getGap(bins, n * bsize);
    }
  };

  // Run the experiment many times and get the average gap for each trial of
  for (int k = 0; k < iterations; k++) {
    for (int i = 0; i < m; i++) {
      computeGap(i, i, lowerGaps);
    }
    for (int i = 0; i < m; i++) {
      computeGap((i + 1) * m, i, higherGaps);
    }
  }

  for (int i = 0; i < lowerGaps.size(); i++) {
    cout << i << " " << lowerGaps[i] / (float)iterations << endl;
  }

  for (int i = 0; i < lowerGaps.size(); i++) {
    int n = (i + 1) * m;
    cout << n << " " << higherGaps[i] / (float)iterations << endl;
  }
}

int main(int argc, char** argv) {

  ExperimentConfiguration configuration;
  if (argc > 1 && std::string(argv[1]) == "-h") {
    cerr << "Example input configuration: " << endl;
    configuration.write(1);
    return 0;
  }

  configuration.read();
  configuration.write(2);
  singleExperiment(configuration);

  stats.print(2);

  cerr << "Done!" << endl;
}
