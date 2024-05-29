#pragma once

#include "../Communication/ICommunicable.h"

namespace DMMSoftware
{
	// classes
	class DMMInterface : public NArray::ICommunicable
	{
		// member functions
	protected:
		void Copy(const DMMInterface& dmm_interface);
	public:
		DMMInterface();
		DMMInterface(const DMMInterface& dmm_interface);
		DMMInterface& operator =(const DMMInterface& dmm_interface);
		virtual std::string Process();
		virtual std::string Process(const std::string& str);
		void Write(std::ostream& out) const;
	};

	// functions
	std::ostream& operator <<(std::ostream& out, const DMMInterface& dmm_interface);
}
