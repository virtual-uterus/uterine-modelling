#ifndef INCLUDE_CONDUCTIVITY_UTERINECONDUCTIVITYMODIFIER_HPP_
#define INCLUDE_CONDUCTIVITY_UTERINECONDUCTIVITYMODIFIER_HPP_

#include <iostream>

#include "Exception.hpp"
#include "AbstractTetrahedralMesh.hpp"
#include "AbstractConductivityModifier.hpp"
#include "distribution_fcts.hpp"

class UterineConductivityModifier : public AbstractConductivityModifier<3, 3> {
 private:
  c_matrix<double, 3, 3> mTensor;
  c_matrix<double, 3, 3> mSpecialMatrix;
  double mCentre;
  double mSlope;
  double mBaseline;
  double mAmplitude;
  std::string mType;
  AbstractTetrahedralMesh<3, 3>* mMesh;

 public:
  UterineConductivityModifier();
  UterineConductivityModifier(double centre, double slope,
                              double baseline, double amplitude,
                              std::string type,
                              AbstractTetrahedralMesh<3, 3>* mesh);
  c_matrix<double, 3, 3>& rCalculateModifiedConductivityTensor(
    unsigned elementIndex,
    const c_matrix<double, 3, 3>& rOriginalConductivity,
    unsigned domainIndex);
  AbstractTetrahedralMesh<3, 3>* GetMesh();
};

#endif  // INCLUDE_CONDUCTIVITY_UTERINECONDUCTIVITYMODIFIER_HPP_
