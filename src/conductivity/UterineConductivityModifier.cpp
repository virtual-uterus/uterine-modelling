#include "../../include/conductivity/UterineConductivityModifier.hpp"


UterineConductivityModifier::UterineConductivityModifier() :
  AbstractConductivityModifier<3, 3>(),
  mSpecialMatrix(zero_matrix<double>(3, 3)), mCentre(0.0), mSlope(1.0),
  mBaseline(0.0), mAmplitude(1.0), mType("linear"), mMesh(NULL)  {
  // Initialise diagonal
  mSpecialMatrix(0, 0) = mBaseline;
  mSpecialMatrix(1, 1) = mBaseline;
  mSpecialMatrix(2, 2) = mBaseline;
}


UterineConductivityModifier::UterineConductivityModifier(
  double centre, double slope, double baseline, double amplitude,
  std::string type, AbstractTetrahedralMesh<3, 3>* mesh) :
  AbstractConductivityModifier<3, 3>(),
  mSpecialMatrix(zero_matrix<double>(3, 3)), mCentre(centre), mSlope(slope),
  mBaseline(baseline), mAmplitude(amplitude), mType(type), mMesh(mesh)  {
  // Initialise diagonal
  mSpecialMatrix(0, 0) = mBaseline;
  mSpecialMatrix(1, 1) = mBaseline;
  mSpecialMatrix(2, 2) = mBaseline;
}


c_matrix<double, 3, 3>& UterineConductivityModifier::rCalculateModifiedConductivityTensor(
  unsigned elementIndex, const c_matrix<double, 3, 3>& rOriginalConductivity,
  unsigned domainIndex) {
  if (elementIndex == 0) {
    // For element 0 let's return the "special matrix"
    return mSpecialMatrix;
  }

  // Get current element centroid to calculate position-based variations
  Element<3, 3>* element = (mMesh->GetElement(elementIndex));
  c_vector<double, 3> cur_centroid = element->CalculateCentroid();

  // Modify the current conductivity
  // along the diagonal save to the "working memory", and return.
  for ( unsigned i=0; i < 3; ++i ) {
    if (mType == "linear") {
      mTensor(i, i) = linear_distribution(cur_centroid(2),
                                           rOriginalConductivity(i, i), mSlope,
                                           mCentre);
    } else if (mType == "gaussian") {
      mTensor(i, i) = gaussian_distribution(cur_centroid(2),
                                             rOriginalConductivity(i, i),
                                             mSlope, mCentre, mAmplitude);
    }
  }
  return mTensor;
}


// Getter
AbstractTetrahedralMesh<3, 3>* UterineConductivityModifier::GetMesh() {
  return mMesh;
}
