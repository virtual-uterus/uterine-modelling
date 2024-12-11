#include "../../include/stimulus/UterineRegionStimulus.hpp"

UterineRegionStimulus::UterineRegionStimulus(std::vector<double> regionProbs) :
      mpCurrentRegion(-1),
      mpRegionProbs(std::move(regionProbs)) {
}


int UterineRegionStimulus::SelectRegion() {
    // Generate a random number between 0 and 1
    double rand_val = RandomNumberGenerator::Instance()->ranf();

    if (rand_val < mpRegionProbs[0]) {
      return 1;  // Ovaries region
    } else if (rand_val < mpRegionProbs[0] + mpRegionProbs[1]) {
      return 2;  // Central region
    } else {
      return 3;  // Cervix region
    }
}

void UterineRegionStimulus::SetRegionProbs(
      const std::vector<double>& regionProbs) {
    mpRegionProbs = regionProbs;
}

int UterineRegionStimulus::GetCurrentRegion() {
    if (mpCurrentRegion == -1) {
        mpCurrentRegion = SelectRegion();
    }
    return mpCurrentRegion;
}
