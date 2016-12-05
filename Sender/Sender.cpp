// Sender.cpp : Defines the entry point for the console application.
//
#include "PrimitiveCollection.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using boost::asio::ip::tcp;

#define MAX_SIZE 104857600 // 100 MB (really MiB) 

void fillPC(PrimitiveCollection* pc, int primtives, int verticies)
{
	for(int prim = 0; prim < primtives; prim++)
	{
		Primitive* p = new Primitive();
		pc->primitives.push_back(p);
		for(int vert = 0; vert < verticies; vert++)
		{
			Vertex* v = new Vertex();
			v->X = 1.0;
			v->Y = 2.0;
			v->Z = 3.0;
			v->XNormal = -1.0;
			v->ZNormal = -3.0;
			p->verticies.push_back(v);
		}
	}
}

int main(int argc, char* argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("remote-host", po::value<std::string>(), "set remote host")
		("remote-port", po::value<unsigned int>(), "set remote port")
		("num-sends", po::value<unsigned int>(), "set number of times to send the collection")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}
	std::string remoteHost = "127.0.0.1";
	if (vm.count("remote-host"))
		remoteHost = vm["remote-host"].as<std::string>();
	unsigned int remotePort = 10301;
	if (vm.count("remote-port"))
		remotePort = vm["remote-port"].as<unsigned int>();
	unsigned int numberOfSends = 1;
	if (vm.count("num-sends"))
		numberOfSends = vm["num-sends"].as<unsigned int>();

	boost::asio::io_service io_service;
	tcp::socket* sock = new tcp::socket(io_service);
	boost::asio::ip::address_v4 ip = boost::asio::ip::address_v4::from_string(remoteHost);
	tcp::endpoint ep(ip, remotePort);
	sock->connect(ep);

	char* buffer = new char[MAX_SIZE];
	for(unsigned int send = 0; send < numberOfSends; send++)
	{
		PrimitiveCollection* pc = new PrimitiveCollection();
		// Each verticy is 20 bytes, send 100 verticies for each primitive, send 1000 primitives = 2MB
		int prim = 1000; 
		int vert = 100;
		fillPC(pc, prim, vert);
		
		int size = 0;
		size = pc->Serialize(buffer+sizeof(size)) + sizeof(size);
		memcpy(buffer, &size, sizeof(size));
		sock->send(boost::asio::buffer(buffer, size));
		std::cout << "Collection sent\n";
	}
	sock->close();

	return 0;
}

