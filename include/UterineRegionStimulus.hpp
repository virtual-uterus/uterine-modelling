#ifndef INCLUDE_UTERINEREGIONSTIMULUS_HPP_
#define INCLUDE_UTERINEREGIONSTIMULUS_HPP_

#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <cmath>

#include "RandomNumberGenerator.hpp"
#include "RegularStimulus.hpp"

class UterineRegionStimulus : public RegularStimulus {
 private:
    double mpStimulusMagnitude;
    double mpStimulusDuration;
    double mpStimulusPeriod;
    double mpStartTime;
    unsigned mpRegion;
    std::vector<double> mpRegionProbs;  // Probabilities for each region

 public:
    UterineRegionStimulus(
                double magnitude,
                double duration,
                double period,
                double startTime,
                std::vector<double> regionProbs);
    void ValidateProbabilities(const std::vector<double>& regionProbs);
    double GetStimulus(double time) override;
    unsigned SelectRegion();
    void SetRegion(unsigned region);
};

#endif  // INCLUDE_UTERINEREGIONSTIMULUS_HPP_


