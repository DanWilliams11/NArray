#pragma once

#include <fstream>
#include <iostream>
#include <process.h>
#include <string>
#include <Windows.h>

namespace NArray
{
	// interface for ensuring an object can be communicated with
	class ICommunicable abstract
	{
		// member data
	public:
		enum CommunicationType
		{
			NoCommunication,
			ReceiveOnly,
			ReceiveSend,
			SendOnly,
			SendReceive,			
		};
		struct CommunicationHelper
		{
			ICommunicable* m_func;
			SOCKET m_sock;
			CommunicationHelper(ICommunicable* func, SOCKET sock)
				: m_func(func), m_sock(sock)
			{
				// all initialisation done in initaliser list
			}
		};
		static const int c_max_num_cons = 50;
		static const int c_max_string_size = 1000;
		static int c_sockaddr_in_size;
		static int c_sockaddr_size;
	private:
		CommunicationType m_communication_type;
		static bool m_continue;
		char* m_local_IP;
		std::fstream m_configuration_file;
		std::string m_configuration_file_name;
		std::fstream m_log_file;
		std::string m_log_file_name;
		unsigned short m_port_num;
		char* m_remote_IP;

		// member functions
	private:
		static void MyReceive(void* v);
		static void MySend(void* v);
		void Receive(SOCKET socket);
		void Send(SOCKET socket);
		void StartReceiving();
		void StartSending();
	protected:
		void Copy(const ICommunicable& communicator);
		void Write(std::ostream& out) const;
	public:
		ICommunicable();
		ICommunicable(const ICommunicable& communicator);
		ICommunicable& operator =(const ICommunicable& communicator);
		static bool GetContinue();
		static void GetError(int error_code);
		static void GetSocketError(SOCKET sock);
		virtual std::string Process() = 0;
		virtual std::string Process(const std::string& str) = 0;
		static void SetContinue(bool cont);
		void SetCommunicationType(CommunicationType communication_type);
		void SetConfigurationFileName(const std::string& configuration_file_name);
		void SetLogFileName(const std::string& log_file_name);
		void SetPortNum(unsigned short port_num);
		void StartCommunicating();
	};
}
