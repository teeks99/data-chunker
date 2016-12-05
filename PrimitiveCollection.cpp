#include "PrimitiveCollection.h"
#include <iostream>
#include <boost/random.hpp>
#include <boost/crc.hpp>

#define CHECKSUM_SIZE 16

PrimitiveCollection::PrimitiveCollection(void)
{
}

PrimitiveCollection::~PrimitiveCollection(void)
{
}

int PrimitiveCollection::Serialize(char* pointerStart)
{
	int size = 0;
	int number = primitives.size();
	int checksum = 0;

	// Leave a blank space for the total size in bytes and a checksum
	size += sizeof(size);
	size += sizeof(checksum);
	// Put in the number of primitives
	memcpy(pointerStart + size, &number, sizeof(number));
	size += sizeof(int);
	
	for(int prim = 0; prim < number; prim++)
	{
		memcpy(pointerStart + size, &prim, sizeof(pointerStart));
		size += sizeof(int);

		size += primitives[prim]->Serialize(pointerStart + size);
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

int PrimitiveCollection::Deserialize(char* pointerStart)
{
	int size = 0;
	int rcvSize = 0;
	int numberOfPrimitives = 0;
	int rcvPrimitive = 0;
	int checksum;
	int rcvChecksum;
	primitives.clear(); // This should already be empty, if not it is a memory leak!

	memcpy(&rcvSize, pointerStart, sizeof(size));
	size += sizeof(size);
	memcpy(&rcvChecksum, pointerStart + size, sizeof(rcvChecksum));
	size += sizeof(rcvChecksum);
	memcpy(&numberOfPrimitives, pointerStart + size, sizeof(numberOfPrimitives));
	size += sizeof(numberOfPrimitives);

	boost::crc_32_type sendCrc;
	int header = sizeof(size) + sizeof(checksum);
	sendCrc.process_bytes( pointerStart + header, rcvSize - header );
	checksum = sendCrc.checksum();

	for(int prim = 0; prim < numberOfPrimitives; prim++)
	{
		memcpy(&rcvPrimitive, pointerStart + size, sizeof(int));
		size += sizeof(rcvPrimitive);
		if(prim != rcvPrimitive)
			std::cout << "Something is wrong!\n";

		Primitive* p = new Primitive();
		size += p->Deserialize(pointerStart + size);

		primitives.push_back(p);
	}

	return size;
}