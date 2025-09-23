#include "../../include/conductivity/UterineTissueModifier.hpp"


/* //default 
UterineTissueModifier::UterineTissueModifier() :
  AbstractConductivityModifier<3, 3>(),
  mSpecialMatrix(zero_matrix<double>(3, 3)), mCentre(0.0), mSteep(1.0),
  mBaseline(0.0), mAmplitude(1.0), mpTissue_dist("linear"), p_mesh(NULL)  {
  // Initialise diagonal
  mSpecialMatrix(0, 0) = mBaseline;
  mSpecialMatrix(1, 1) = mBaseline;
  mSpecialMatrix(2, 2) = mBaseline;
} */

void TissueModLoadParms() {

  std::cout << "TissueModLoadParms..." << std::endl;


}