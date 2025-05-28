#include "../../include/UterineConductivityModifier.hpp"
#include "Exception.hpp"


UterineConductivityModifier::UterineConductivtiyModifier() :
  AbstractConductivityModifier<3, 3>(),
  mSpecialMatrix(zero_matrix<double>(3, 3)), mCentre(0.0), mSlope(1.0),
  mBaseline(0.0)  {
  // Initialise diagonal
  mSpecialMatrix(0, 0) = 1.0;
  mSpecialMatrix(1, 1) = 1.0;
  mSpecialMatrix(2, 2) = 1.0;
}


UterineConductivityModifier::UterineConductivtiyModifier(
  double centre, double slope, double baseline) :
  AbstractConductivityModifier<3, 3>(),
  mSpecialMatrix(zero_matrix<double>(3, 3)), mCentre(centre), mSlope(slope),
  mBaseline(baseline)  {
  // Initialise diagonal
  mSpecialMatrix(0, 0) = 1.0;
  mSpecialMatrix(1, 1) = 1.0;
  mSpecialMatrix(2, 2) = 1.0;
}


c_matrix<double, 3, 3>& UterineConductivityModifier::rCalculateModifiedConductivityTensor(
  unsigned elementIndex, const c_matrix<double, 3, 3>& rOriginalConductivity,
  unsigned domainIndex) {
  if (elementIndex == 0) {
    // For element 0 let's return the "special matrix"
    return mSpecialMatrix;
  }

  // Get current element centroid to calculate position-based variations
  Element<3, 3>* p_element = (p_mesh->GetElement(elementIndex));
  c_vector<double, 3> cur_centroid = p_element->CalculateCentroid();

  // Modify the current conductivity according to Gaussian distribution
  // along the diagonal save to the "working memory", and return.
  for ( unsigned i=0; i < 3; ++i ) {
    double gaussian_modifier;  // Change this to be called from the mods file
    mTensor(i, i) = elementIndex*rOriginalConductivity(i, i)*gaussian_modifier;
  }
  return mTensor;
  }
}
