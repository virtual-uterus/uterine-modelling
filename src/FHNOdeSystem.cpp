#include "FHNOdeSystem.hpp"
#include "Exception.hpp"

FHNOdeSystem::FHNOdeSystem() : AbstractOdeSystem(2)
{
	mpSystemInfo = OdeSystemInformation<FHNOdeSystem>::Instance();

	// Initialise parameters with default config files
	ReadConfigParams(FHN_ODE_SYSTEM_CONSTANTS::CONFIG_PATH);
}


FHNOdeSystem::FHNOdeSystem(std::string param_config_file)
	: AbstractOdeSystem(2)
{
	mpSystemInfo = OdeSystemInformation<FHNOdeSystem>::Instance();

	ReadConfigParams(param_config_file);
}

void FHNOdeSystem::EvaluateYDerivatives(double time, 
	const std::vector<double>& rY, std::vector<double>& rDY)
{
	double beta;

	if (mSlow_wave)
	{
 		beta = mC * sin(2*M_PI*mFreq*time); // Define the beta function
	}

	else
	{
		beta = mBeta;
	}

	// rY[0] = u and rY[1] = r
	rDY[0] = mA*rY[0] * (rY[0] - beta) * (mDelta - rY[0]) - mB * rY[1] + mStim;
	rDY[1] = mEpsilon * (rY[0] - mGamma*rY[1]);
}


void FHNOdeSystem::ReadConfigParams(std::string config_file)
{
	std::string table_name;
	const auto params = toml::parse(config_file);

	mSlow_wave = toml::find<bool>(params, "slow_wave");

	if (mSlow_wave)
	{
		table_name = "FHNSlowWave";
	}

	else
	{
		table_name = "FHNOriginal";
	}

	const auto& FHNOdeSystemParams = toml::find(params, table_name);

	mA = toml::find<double>(FHNOdeSystemParams, "A");
	mB = toml::find<double>(FHNOdeSystemParams, "B");
	mC = toml::find<double>(FHNOdeSystemParams, "C");
	mFreq = toml::find<double>(FHNOdeSystemParams, "f");
	mBeta = toml::find<double>(FHNOdeSystemParams, "beta");
	mDelta = toml::find<double>(FHNOdeSystemParams, "delta");
	mEpsilon = toml::find<double>(FHNOdeSystemParams, "epsilon");
	mGamma = toml::find<double>(FHNOdeSystemParams, "gamma");
	mStim = toml::find<double>(FHNOdeSystemParams, "Istim");
	
}


void FHNOdeSystem::PrintParams()
{
	// Print all parameters for testing purposes
	std::cout << "mA = " << mA << "\n";
	std::cout << "mB = " << mB << "\n";
	std::cout << "mC = " << mC << "\n";
	std::cout << "mFreq = " << mFreq << "\n";
	std::cout << "mBeta = " << mBeta << "\n";
	std::cout << "mDelta = " << mDelta << "\n";
	std::cout << "mEpsilon = " << mEpsilon << "\n";
	std::cout << "mGamma = " << mGamma << "\n";
	std::cout << "mStim = " << mStim << "\n";
	std::cout << "mSlow_wave = " << mSlow_wave << "\n";
}

template<>
void OdeSystemInformation<FHNOdeSystem>::Initialise()
{
	// FHN source variable
    this->mVariableNames.push_back("u");
    this->mVariableUnits.push_back("mV");
    this->mInitialConditions.push_back(-1.0);

	// FHN recovery variable
	this->mVariableNames.push_back("r");
	this->mVariableUnits.push_back("mV");
	this->mInitialConditions.push_back(0.0);

    this->mInitialised = true;
}

