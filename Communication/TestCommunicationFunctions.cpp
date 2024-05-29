#include <fstream>

#include "../CommunicationExamples/DMMInterface.h"
#include "../Core/NArrayImplementation.h"
#include "CommunicationFunctions.h"
#include "TestCommunicationFunctions.h"

void NArray::TestCommunicationFunctions(NArraySupport::StorageOption storage_option, std::fstream& output_file)
{
	output_file << "*** Testing " << NArraySupport::StorageOptionStr(storage_option) << " allocated memory: ";
	output_file << "Communication Functions" << std::endl << std::endl;

	int num_dmm_controllers = 2;
	Shape shape(c_matrix_order, num_dmm_controllers, num_dmm_controllers);

	typedef DMMSoftware::DMMInterface stored_type;
	typedef NArrayType<stored_type>::Matrix storage_type;
	storage_type dmm_controllers(storage_option, shape, stored_type(), false);
	dmm_controllers[1][1][1].SetCommunicationType(ICommunicable::SendReceive);
	dmm_controllers[1][1][1].SetLogFileName("CommunicationExamples/dmm1_txrx_log.txt");
	dmm_controllers[1][1][1].SetPortNum(5101);
	dmm_controllers[1][2][1].SetCommunicationType(ICommunicable::ReceiveSend);
	dmm_controllers[1][2][1].SetLogFileName("CommunicationExamples/dmm1_rxtx_log.txt");
	dmm_controllers[1][2][1].SetPortNum(5101);
	dmm_controllers[2][1][1].SetCommunicationType(ICommunicable::SendReceive);
	dmm_controllers[2][1][1].SetLogFileName("CommunicationExamples/dmm2_txrx_log.txt");
	dmm_controllers[2][1][1].SetPortNum(5102);
	dmm_controllers[2][2][1].SetCommunicationType(ICommunicable::ReceiveSend);
	dmm_controllers[2][2][1].SetLogFileName("CommunicationExamples/dmm2_rxtx_log.txt");
	dmm_controllers[2][2][1].SetPortNum(5102);
	CommunicateValues<storage_type, stored_type>(dmm_controllers);
}
