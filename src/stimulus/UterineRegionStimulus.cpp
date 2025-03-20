#include "../../include/stimulus/UterineRegionStimulus.hpp"

UterineRegionStimulus::UterineRegionStimulus(
    double magnitude, double duration, double period, double start,
    boost::shared_ptr<UterineRegionSelector> selector) :
      RegularStimulus(magnitude, duration, period, start),
      mpRegion(0),
      mpSelector(selector) {
}


double UterineRegionStimulus::GetStimulus(double time) {
    if (time < mStartTime) {
        return 0.0;
    }

    double beatTime = fmod(time-mStartTime, mPeriod);

    if (beatTime >=0 && beatTime <= mDuration) {
        unsigned region = mpSelector->GetCurrentRegion();

        if (region == mpRegion) {
            return mMagnitudeOfStimulus;
        }
    } else {
        mpSelector->SetCurrentRegion(0);
        return 0.0;
    }
    return 0.0;
}


void UterineRegionStimulus::SetRegionProbs(
  const std::vector<double> region_probs) {
    mpSelector->SetRegionProbs(region_probs);
}


void UterineRegionStimulus::SetRegion(unsigned region) {
    mpRegion = region;
}
