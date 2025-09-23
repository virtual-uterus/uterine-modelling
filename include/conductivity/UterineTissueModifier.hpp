#ifndef INCLUDE_CONDUCTIVITY_UTERINETISSUEMODIFIER_HPP_
#define INCLUDE_CONDUCTIVITY_UTERINETISSUEMODIFIER_HPP_

#include <iostream>

#include "Exception.hpp"
#include "AbstractTetrahedralMesh.hpp"
#include "AbstractConductivityModifier.hpp"
#include "distribution_fcts.hpp"

//following https://chaste.github.io/docs/user-tutorials/bidomainwithconductivitymodifier/
class UterineTissueModifier : public AbstractConductivityModifier<3,3> 
{
private:

    c_matrix<double,3,3> mTensor;
    c_matrix<double,3,3> mSpecialMatrix;





    
public:

    //for gaussian distribution along z
    double mCentre;
    double mSteep;
    double mBaseline;
    double mAmplitude;
    //double mMin;

    AbstractTetrahedralMesh<3,3>* p_mesh;

    std::string mpTissue_dist;  // Conductivity distn - tissue wide
    std::unordered_map<std::string, float> mpTissue_parameters;    

    //method for parsing distribution parameters
    //void SetTissueModParms();

    //mTensor(0,0) = 0;

//    GaussianConductivityModifier()
    UterineTissueModifier()
        : AbstractConductivityModifier<3,3>(),
          mSpecialMatrix( zero_matrix<double>(3,3) )
          {
              mSpecialMatrix(0,0) = 1.0;
              mSpecialMatrix(1,1) = 1.0;
              mSpecialMatrix(2,2) = 1.0;
          }    
    //UterineTissueModifier();
/*    UterineTissueModifier(double centre, double steep,
                              double baseline, double amplitude,                              
                              std::string type,
                              AbstractTetrahedralMesh<3, 3>* mesh);     */

    void TissueModLoadParms();

    c_matrix<double,3,3>& rCalculateModifiedConductivityTensor(unsigned elementIndex, 
        const c_matrix<double,3,3>& rOriginalConductivity, unsigned domainIndex)
    {

        if (elementIndex == 0)
        {
            // For element 0 let's return the "special matrix", regardless of intra/extracellular.
            return mSpecialMatrix;
        }

        //get this element's centroid -- used for calculating position-based variation
        Element<3,3>* p_element = (p_mesh->GetElement(elementIndex));
        c_vector<double, 3> cur_centroid = p_element->CalculateCentroid();

        for ( unsigned i=0; i<3; i++ )
        {
            //for some reason, chaste divides the conductances by the...elementIndex...so we multiply here
            //mTensor(i,i) = elementIndex*rOriginalConductivity(i,i)*(mBaseline + mAmplitude*exp(-mSteep*pow((cur_centroid(2)-mCentre),2.0)));

            //testing without multiplying by index
            mTensor(i,i) = rOriginalConductivity(i,i)*(mBaseline + mAmplitude*exp(-mSteep*pow((cur_centroid(2)-mCentre),2.0)));



        }          
        //debug
        /* std::cout << "(UterineTissueModifer.hpp) cur_centroid(2): " << cur_centroid(2) << " mTensor: " 
            << mTensor(0,0)/elementIndex << "," << mTensor(1,1)/elementIndex << "," << mTensor(2,2)/elementIndex << std::endl;
 */
        return mTensor;
    }
};

#endif  // INCLUDE_CONDUCTIVITY_UTERINETISSUEMODIFIER_HPP_

