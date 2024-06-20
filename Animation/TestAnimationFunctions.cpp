#include <fstream>

#include "../AnimationExamples/DisappearingRisingFloatingInflatingBalloon.h"
#include "../AnimationExamples/LorenzAttractor.h"
#include "../Core/NArrayImplementation.h"
#include "AnimationFunctions.h"
#include "TestAnimationFunctions.h"

void NArray::TestAnimationFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file)
{
	std::cout << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	std::cout << "Animation Functions" << std::endl << std::endl;
	output_file << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Animation Functions" << std::endl << std::endl;

	std::cout << "Animation in 0D: Inflating Balloon" << std::endl;
	typedef ChaoticDynamicalSystems::InflatingBalloon stored_type1;
	typedef NArrayType<stored_type1>::Scalar storage_type1;
	stored_type1 inflating_balloon;
	storage_type1 inflating_balloons(storage_option, inflating_balloon);
	Animation0D<storage_type1, stored_type1>::GetTheInstance().Animate(&inflating_balloons, NArraySupport::UseDataToSize, 10, 10, 1000, 1000);

	std::cout << "Animation in 1D: Floating Inflating Balloon" << std::endl;
	typedef ChaoticDynamicalSystems::FloatingInflatingBalloon stored_type2;
	typedef NArrayType<stored_type2>::Scalar storage_type2;
	stored_type2 floating_inflating_balloon;
	storage_type2 floating_inflating_balloons(storage_option, floating_inflating_balloon);
	Animation1D<storage_type2, stored_type2>::GetTheInstance().Animate(&floating_inflating_balloons, NArraySupport::UseDataToSize, 10, 10, 1000, 1000);

	std::cout << "Animation in 2D: Rising Floating Inflating Balloon" << std::endl;
	typedef ChaoticDynamicalSystems::RisingFloatingInflatingBalloon stored_type3;
	typedef NArrayType<stored_type3>::Scalar storage_type3;
	stored_type3 rising_floating_inflating_balloon;
	storage_type3 rising_floating_inflating_balloons(storage_option, rising_floating_inflating_balloon);
	Animation2D<storage_type3, stored_type3>::GetTheInstance().Animate(&rising_floating_inflating_balloons, NArraySupport::UseDataToSize, 10, 10, 1000, 1000);

	std::cout << "Animation in 3D: Disappearing Rising Floating Inflating Balloon" << std::endl;
	typedef ChaoticDynamicalSystems::DisappearingRisingFloatingInflatingBalloon stored_type4;
	typedef NArrayType<stored_type4>::Scalar storage_type4;
	stored_type4 disappearing_rising_floating_inflating_balloon;
	storage_type4 disappearing_rising_floating_inflating_balloons(storage_option, disappearing_rising_floating_inflating_balloon);
	Animation3D<storage_type4, stored_type4>::GetTheInstance().Animate(&disappearing_rising_floating_inflating_balloons, NArraySupport::UseDataToSize, 10, 10, 1000, 1000);

	std::cout << "Animation in 3D: Lorenz Attractor" << std::endl;
	typedef ChaoticDynamicalSystems::LorenzAttractor stored_type5;
	typedef NArrayType<stored_type5>::Vector storage_type5;
	stored_type5::ResetGlobalCount();
	stored_type5 lorenz_attractor;
	int num_lorenz_attractors = 10000;
	Shape shape(c_vector_order, num_lorenz_attractors);
	storage_type5 lorenz_attractors(storage_option, shape, lorenz_attractor, false);
	Animation3D<storage_type5, stored_type5>::GetTheInstance().Animate(&lorenz_attractors, NArraySupport::UseDataToSizeAndColour, 10, 10, 1000, 1000);
}
