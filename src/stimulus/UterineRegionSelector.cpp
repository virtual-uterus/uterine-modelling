#include "../../include/stimulus/UterineRegionSelector.hpp"

UterineRegionSelector::UterineRegionSelector() :
      mpCurrentRegion(0),
      mpOvariesProb(1.0),
      mpCentreProb(0.0),
      mpCervicalProb(0.0) {
}


double UterineRegionSelector::GetStimulus(double time) {
    return 0.0;
}


unsigned UterineRegionSelector::SelectRegion() {
    // Generate a random number between 0 and 1
    static std::mt19937 mt(58);  // Random number generator
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double rand_val = dist(mt);

    if (rand_val < mpOvariesProb) {
      return 1;  // Ovaries region
    } else if (rand_val < (mpOvariesProb + mpCentreProb)) {
      return 2;  // Central region
    } else {
      return 3;  // Cervix region
    }
}


void UterineRegionSelector::SetRegionProbs(
      const std::vector<double>& region_probs) {
    mpOvariesProb = region_probs[0];
    mpCentreProb = region_probs[1];
    mpCervicalProb = region_probs[2];
}


unsigned UterineRegionSelector::GetCurrentRegion() {
    if (mpCurrentRegion == 0) {
        mpCurrentRegion = SelectRegion();
    }
    return mpCurrentRegion;
}


void UterineRegionSelector::SetCurrentRegion(unsigned region) {
    mpCurrentRegion = region;
}


// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(UterineRegionSelector)
