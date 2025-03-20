#ifndef INCLUDE_STIMULUS_UTERINEREGIONSELECTOR_HPP_
#define INCLUDE_STIMULUS_UTERINEREGIONSELECTOR_HPP_

#include <iostream>
#include <utility>
#include <algorithm>
#include <random>
#include <vector>
#include <cmath>

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "AbstractStimulusFunction.hpp"

class UterineRegionSelector : public AbstractStimulusFunction {
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version) {
        // This calls serialize on the base class.
        archive & boost::serialization::base_object<AbstractStimulusFunction>(*this);
        archive & mpCurrentRegion;
        archive & mpOvariesProb;
        archive & mpCentreProb;
        archive & mpCervicalProb;
    }
 private:
    unsigned mpCurrentRegion;  // Current region for stimulus
    double mpOvariesProb;
    double mpCentreProb;
    double mpCervicalProb;

 public:
    UterineRegionSelector();
    double GetStimulus(double time) override;
    unsigned SelectRegion();
    void SetRegionProbs(const std::vector<double>& region_probs);
    unsigned GetCurrentRegion();
    void SetCurrentRegion(unsigned region);
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(UterineRegionSelector)
namespace boost {
namespace serialization {
template<class Archive>
inline void load_construct_data(
    Archive & ar, UterineRegionSelector * t, const unsigned int file_version) {
     ::new(t)UterineRegionSelector();
}
}
}  // namespace boost

#endif  // INCLUDE_STIMULUS_UTERINEREGIONSELECTOR_HPP_


