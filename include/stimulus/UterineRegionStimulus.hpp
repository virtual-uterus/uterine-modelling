#ifndef INCLUDE_UTERINEREGIONSTIMULUS_HPP_
#define INCLUDE_UTERINEREGIONSTIMULUS_HPP_

#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <cmath>

#include "RandomNumberGenerator.hpp"

class UterineRegionStimulus {
 private:
    int mpCurrentRegion;  // Current region for stimulus
    std::vector<double> mpRegionProbs;  // Probabilities for each region

 public:
    UterineRegionStimulus(std::vector<double> regionProbs);
    int SelectRegion();
    void SetRegionProbs(const std::vector<double>& regionProbs);
    int GetCurrentRegion();
};

#endif  // INCLUDE_UTERINEREGIONSTIMULUS_HPP_


