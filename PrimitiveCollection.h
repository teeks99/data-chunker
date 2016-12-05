#ifndef PRIMITIVE_COLLECTION_H
#define PRIMITIVE_COLLECTION_H

#include <vector>
#include "Primitive.h"

class PrimitiveCollection
{
public:
	PrimitiveCollection(void);
	~PrimitiveCollection(void);

	std::vector<Primitive*> primitives;

	int Serialize(char* pointerStart);
	int Deserialize(char* pointerStart);
};

#endif //PRIMITIVE_COLLECTION_H