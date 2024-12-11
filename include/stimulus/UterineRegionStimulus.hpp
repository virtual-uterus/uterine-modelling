#ifndef INCLUDE_STIMULUS_UTERINEREGIONSTIMULUS_HPP_
#define INCLUDE_STIMULUS_UTERINEREGIONSTIMULUS_HPP_

#include <iostream>
#include <utility>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "RegularStimulus.hpp"
#include "UterineRegionSelector.hpp"

class UterineRegionStimulus : public RegularStimulus {
 private:
    double mpRegion;
    boost::shared_ptr<UterineRegionSelector> mpSelector;

 public:
    UterineRegionStimulus(
      double magnitude, double duration, double period, double start,
      boost::shared_ptr<UterineRegionSelector> selector);
    double GetStimulus(double time) override;
    void SetRegionProbs(const std::vector<double> regionProbs);
    void SetRegion(const unsigned region);
};

#endif  // INCLUDE_STIMULUS_UTERINEREGIONSTIMULUS_HPP_
