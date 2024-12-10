#include "../include/UterineRegionStimulus.hpp"

UterineRegionStimulus::UterineRegionStimulus(
    double magnitude, double duration, double period, double start, std::vector<double> regionProbs)
    : RegularStimulus(magnitude, duration, period, start),
      mpRegion(0),
      mpRegionProbs(std::move(regionProbs)) {
}

double UterineRegionStimulus::GetStimulus(double time) {
    double current_time = time - mpStartTime;
    if (current_time < 0.0 || std::fmod(current_time, mpStimulusPeriod) > mpStimulusDuration) {
        return 0.0;
    }

    // Select region based on probabilities
    unsigned region = SelectRegion();

    // Logic to determine if the current cell is in the selected region
    if (region == mpRegion) {
        return mpStimulusMagnitude;
    }

    return 0.0;
}

unsigned UterineRegionStimulus::SelectRegion() {
    // Generate a random number between 0 and 1
    double rand_val = RandomNumberGenerator::Instance()->ranf();

    if (rand_val < mpRegionProbs[0]) {
      return 0;  // Ovaries region
    } else if (rand_val < mpRegionProbs[0] + mpRegionProbs[1]) {
      return 1;  // Central region
    } else {
      return 2;  // Cervix region
    }
}

void UterineRegionStimulus::SetRegion(unsigned region) {
    mpRegion = region;
}
