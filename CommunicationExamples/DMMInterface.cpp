#include "DMMInterface.h"

using namespace DMMSoftware;

// member functions

DMMInterface::DMMInterface()
{
}

DMMInterface::DMMInterface(const DMMInterface& dmm_interface)
{
	Copy(dmm_interface);
}

DMMInterface& DMMInterface::operator =(const DMMInterface& dmm_interface)
{
	Copy(dmm_interface);
	return *this;
}

void DMMInterface::Copy(const DMMInterface& dmm_interface)
{
	ICommunicable::Copy(dmm_interface);
}

std::string DMMInterface::Process()
{
	return "Hello John";
}

std::string DMMInterface::Process(const std::string& str)
{
	if (str == "Hello John")
		return "Hello Janet";
	else if (str == "Hello Janet")
		return "Hello John";
	else
		throw;
}

void DMMInterface::Write(std::ostream& out) const
{
	ICommunicable::Write(out);
}

// functions

std::ostream& DMMSoftware::operator <<(std::ostream& out, const DMMInterface& dmm_interface)
{
	dmm_interface.Write(out);
	return out;
}
