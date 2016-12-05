// Receiver.cpp : Defines the entry point for the console application.
//
#include "PrimitiveCollection.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using boost::asio::ip::tcp;

#define MAX_SIZE 104857600 // 100 MB (really MiB) 

int main(int argc, char* argv[])
{
	PrimitiveCollection* pc;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("local-port", po::value<unsigned int>(), "set local port")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}
	
	unsigned int localPort = 10301;
	if (vm.count("local-port"))
		localPort = vm["local-port"].as<unsigned int>();

	char* buffer = new char[MAX_SIZE];
	boost::asio::io_service io_service;

	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), localPort));
	for (;;)
	{
		boost::shared_ptr<tcp::socket> sock(new tcp::socket(io_service));
		a.accept(*sock);

		while(sock->is_open())
		{
			boost::system::error_code error;
			int lengthRead = 0;
			while(!lengthRead)
			{
				lengthRead = sock->read_some(
					boost::asio::buffer(buffer, MAX_SIZE), error);
			}

			if(lengthRead > 0)
			{
				int expectedLength = 0;
				memcpy(&expectedLength, buffer, sizeof(expectedLength));

				while(lengthRead < expectedLength)
				{
				 lengthRead += sock->read_some(
					boost::asio::buffer(buffer + lengthRead, 
					MAX_SIZE - lengthRead), error);
				}
				pc = new PrimitiveCollection();
				pc->Deserialize(buffer + sizeof(expectedLength));
				std::cout << "Collection received\n";
			}
		}
	}

	return 0;
}

