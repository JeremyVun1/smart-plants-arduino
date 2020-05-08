#ifndef UTIL
#define UTIL

#include <MFRC522.h>

double rollingAverage (double avg, double new_val, int n) {
  if (n == 1)
    return new_val;
  else {
    avg -= avg / n;
    avg += new_val / 2;
    return avg;
  }
}

#endif
