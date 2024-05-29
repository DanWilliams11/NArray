#include "ICommunicable.h"

using namespace NArray;

int ICommunicable::c_sockaddr_in_size = sizeof(struct sockaddr_in);
int ICommunicable::c_sockaddr_size = sizeof(struct sockaddr);
bool ICommunicable::m_continue = true;

ICommunicable::ICommunicable()
{
	m_communication_type = NoCommunication;
	m_local_IP = nullptr;
	m_configuration_file_name = "configuration.txt";
	m_log_file_name = "log.txt";
	m_port_num = 5100;
	m_remote_IP = nullptr;
}

ICommunicable::ICommunicable(const ICommunicable& communicator)
{
	Copy(communicator);
}

ICommunicable& ICommunicable::operator =(const ICommunicable& communicator)
{
	Copy(communicator);
	return *this;
}

void ICommunicable::Copy(const ICommunicable& communicator)
{
	m_communication_type = communicator.m_communication_type;
	m_local_IP = communicator.m_local_IP;
	m_configuration_file_name = communicator.m_configuration_file_name;
	m_log_file_name = communicator.m_log_file_name;
	m_port_num = communicator.m_port_num;
	m_remote_IP = communicator.m_remote_IP;
}

bool ICommunicable::GetContinue()
{
	return m_continue;
}

void ICommunicable::GetError(int error_code)
{
	char msg[c_max_string_size];
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, 0, msg, c_max_string_size, nullptr);
	std::cerr << msg;
}

void ICommunicable::GetSocketError(SOCKET sock)
{
	if (sock == SOCKET_ERROR)
		GetError(WSAGetLastError());
}

void ICommunicable::MyReceive(void* v)
{
	CommunicationHelper* ch = (CommunicationHelper*) v;
	ICommunicable* func = ch->m_func;
	SOCKET sock = ch->m_sock;
	func->Receive(sock);
}

void ICommunicable::MySend(void* v)
{
	CommunicationHelper* ch = (CommunicationHelper*) v;
	ICommunicable* func = ch->m_func;
	SOCKET sock = ch->m_sock;
	func->Send(sock);
}

void ICommunicable::Receive(SOCKET socket)
{
	char recv_str[c_max_string_size];
	memset(recv_str, 0, c_max_string_size);
	GetSocketError(recv(socket, recv_str, c_max_string_size, 0));
	std::cout << "Received: " << recv_str << std::endl;
	m_log_file << "Received: " << recv_str << std::endl;
	m_log_file.flush();
	if (m_communication_type == ReceiveSend)
	{
		std::string send_str = Process(recv_str);
		GetSocketError(send(socket, send_str.c_str(), send_str.length(), 0));
		std::cout << "Sent: " << send_str << std::endl;
		m_log_file << "Sent: " << send_str << std::endl;
		m_log_file.flush();
	}
	GetSocketError(closesocket(socket));
}

void ICommunicable::SetContinue(bool cont)
{
	m_continue = cont;
}

void ICommunicable::SetCommunicationType(CommunicationType communication_type)
{
	m_communication_type = communication_type;
}

void ICommunicable::SetConfigurationFileName(const std::string& configuration_file_name)
{
	m_configuration_file_name = configuration_file_name;
	m_configuration_file.open(m_configuration_file_name, std::ios_base::in);
}

void ICommunicable::SetLogFileName(const std::string& log_file_name)
{
	m_log_file_name = log_file_name;
	m_log_file.open(m_log_file_name, std::ios_base::out);
}

void ICommunicable::SetPortNum(unsigned short port_num)
{
	m_port_num = port_num;
}

void ICommunicable::Send(SOCKET socket)
{
	std::string send_str = Process();
	GetSocketError(send(socket, send_str.c_str(), send_str.length(), 0));
	std::cout << "Sent: " << send_str << std::endl;
	m_log_file << "Sent: " << send_str << std::endl;
	m_log_file.flush();
	if (m_communication_type == SendReceive)
	{
		char recv_str[c_max_string_size];
		memset(recv_str, 0, c_max_string_size);
		GetSocketError(recv(socket, recv_str, c_max_string_size, 0));
		std::cout << "Received: " << recv_str << std::endl;
		m_log_file << "Received: " << recv_str << std::endl;
		m_log_file.flush();
	}
	GetSocketError(closesocket(socket));
}

void ICommunicable::StartCommunicating()
{
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	GetError(WSAStartup(version, &data));

	hostent* local_host = gethostbyname("");
	m_local_IP = inet_ntoa(*(struct in_addr *)*local_host->h_addr_list);
	m_remote_IP = m_local_IP;

	if (m_communication_type == ReceiveOnly || m_communication_type == ReceiveSend)
		StartReceiving();
	else if (m_communication_type == SendOnly || m_communication_type == SendReceive)
		StartSending();
}

void ICommunicable::StartReceiving()
{
	SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	GetSocketError(listen_socket);

	sockaddr_in listen_addr;
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = inet_addr(m_local_IP);
	listen_addr.sin_port = htons(m_port_num);

	GetSocketError(bind(listen_socket, (SOCKADDR*) &listen_addr, c_sockaddr_in_size));
	GetSocketError(listen(listen_socket, c_max_num_cons));

	while (ICommunicable::GetContinue())
	{
		sockaddr rx_addr;
		SOCKET rx_socket = accept(listen_socket, &rx_addr, &c_sockaddr_size);
		GetSocketError(rx_socket);

		if (rx_socket != SOCKET_ERROR)
		{
			CommunicationHelper ch(this, rx_socket);
			_beginthread(MyReceive, 0, &ch);
		}

		Sleep(2000);
	}

	GetSocketError(closesocket(listen_socket));
}

void ICommunicable::StartSending()
{
	sockaddr_in tx_addr;
	tx_addr.sin_family = AF_INET;
	tx_addr.sin_addr.s_addr = inet_addr(m_remote_IP);
	tx_addr.sin_port = htons(m_port_num);

	while (ICommunicable::GetContinue())
	{
		SOCKET tx_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		GetSocketError(tx_socket);

		int ret = connect(tx_socket, (SOCKADDR*) &tx_addr, c_sockaddr_in_size);
		GetSocketError(ret);

		if (ret != SOCKET_ERROR)
		{
			CommunicationHelper ch(this, tx_socket);
			_beginthread(MySend, 0, &ch);
		}

		Sleep(2000);
	}
}

void ICommunicable::Write(std::ostream& out) const
{
	out << "Communication Type: " << m_communication_type << std::endl;
	out << "Log File Name: " << m_log_file_name << std::endl;
}
