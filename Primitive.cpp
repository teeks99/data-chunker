#include "Primitive.h"
#include <iostream>
#include <boost/random.hpp>
#include <boost/crc.hpp>

Primitive::Primitive(void)
{
}

Primitive::~Primitive(void)
{
}

int Primitive::Serialize(char* pointerStart)
{
	int size = 0;
	int number = verticies.size();
	int checksum = 0;

	// Leave a blank space for the total size in bytes and a checksum
	size += sizeof(size);
	size += sizeof(checksum);
	// Put in the number of verticies
	memcpy(pointerStart + size, &number, sizeof(number));
	size += sizeof(int);

	for(int vert = 0; vert < number; vert++)
	{
		memcpy(pointerStart + size, &vert, sizeof(pointerStart));
		size += sizeof(int);

		memcpy(pointerStart + size, verticies[vert], sizeof(Vertex));
		size += sizeof(Vertex);
	}
	// Run the checksum on pointerStart + (sizeof(int)+sizeof(checksum))
	boost::crc_32_type sendCrc;
	int header = sizeof(size) + sizeof(checksum);
	sendCrc.process_bytes( pointerStart + header, size - header );
	checksum = sendCrc.checksum();

	// Put in the total size in bytes
	memcpy(pointerStart, &size, sizeof(size));
	memcpy(pointerStart + sizeof(size), &checksum, sizeof(checksum));

	return size;
}

int Primitive::Deserialize(char* pointerStart)
{
	int size = 0;
	int rcvSize = 0;
	int numberOfVerticies = 0;
	int rcvVertex = 0;
	int checksum;
	int rcvChecksum;
	verticies.clear(); // This should already be empty, if not it is a memory leak!

	memcpy(&rcvSize, pointerStart, sizeof(size));
	size += sizeof(size);
	memcpy(&rcvChecksum, pointerStart + size, sizeof(rcvChecksum));
	size += sizeof(rcvChecksum);
	memcpy(&numberOfVerticies, pointerStart + size, sizeof(numberOfVerticies));
	size += sizeof(numberOfVerticies);

	boost::crc_32_type sendCrc;
	int header = sizeof(size) + sizeof(checksum);
	sendCrc.process_bytes( pointerStart + header, rcvSize - header );
	checksum = sendCrc.checksum();
	if(checksum != rcvChecksum)
		std::cout << "Something is wrong!\n";

	for(int vert = 0; vert < numberOfVerticies; vert++)
	{
		memcpy(&rcvVertex, pointerStart + size, sizeof(int));
		size += sizeof(rcvVertex);
		if(vert != rcvVertex)
			std::cout << "Something is wrong!\n";

		Vertex* v = new Vertex();
		memcpy(v, pointerStart+size, sizeof(Vertex));
		size += sizeof(Vertex);

		verticies.push_back(v);
	}

	return size;
}