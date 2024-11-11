#pragma once
#include <stdio.h>
struct ExperimentConfiguration {
  int   m;
  int   d;
  int   useSingleAllocation;
  int   useBetaProbability;
  float betaProbability;
  int   iterations;
  int   k;
  int   outputLoad;
  int   partialInformation;

  ExperimentConfiguration() {
    betaProbability     = 0.0f;
    iterations          = 1000;
    useSingleAllocation = true;
    useBetaProbability  = false;
    betaProbability     = 1;
    m                   = 30;
    d                   = 1;
    k                   = 1;
    outputLoad          = 0;
    partialInformation  = 0;
  }

  inline void read() {
    scanf("m = %d\n", &m);
    scanf("d = %d\n", &d);
    scanf("betaProbability = %f\n", &betaProbability);
    scanf("iterations = %d\n", &iterations);
    scanf("useSingleAllocation = %d\n", &useSingleAllocation);
    scanf("useBetaProbability = %d\n", &useBetaProbability);
    scanf("k = %d\n", &k);
    scanf("outputLoad = %d\n", &outputLoad);
    scanf("partialInformation = %d\n", &partialInformation);
  }

  inline void write(int fd) {
    dprintf(fd, "m = %d\n", m);
    dprintf(fd, "d = %d\n", d);
    dprintf(fd, "betaProbability = %f\n", betaProbability);
    dprintf(fd, "iterations = %d\n", iterations);
    dprintf(fd, "useSingleAllocation = %d\n", useSingleAllocation);
    dprintf(fd, "useBetaProbability = %d\n", useBetaProbability);
    dprintf(fd, "k = %d\n", k);
    dprintf(fd, "outputLoad = %d\n", outputLoad);
    dprintf(fd, "partialInformation = %d\n", partialInformation);
  }
};
