#ifndef INCLUDE_UTERINECONDUCTIVITYMODIFIER_HPP_
#define INCLUDE_UTERINECONDUCTIVITYMODIFIER_HPP_

#include <iostream>
#include "Exception.hpp"
#include "AbstractConductivityModifier.hpp"

class UterineConductivityModifier : public AbstractConductivityModifier<3, 3> {
 private:
  c_matrix<double, 3, 3> mTensor;
  c_matrix<double, 3, 3> mSpecialMatrix;
  double mCentre;
  double mSlope;
  double mBaseline;

 public:
  UterineConductivityModifier();
  UterineConductivityModifier(double centre, double slope, double baseline);
  c_matrix<double, 3, 3>& rCalculateModifiedConductivityTensor(
    unsigned elementIndex,
    const c_matrix<double, 3, 3>& rOriginalConductivity,
    unsigned domainIndex);
};

#endif  // INCLUDE_UTERINECONDUCTIVITYMODIFIER_HPP_
