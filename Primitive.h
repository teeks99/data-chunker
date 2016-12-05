#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>

class Vertex
{
public:
	float X;
	float Y;
	float Z;
	float XNormal;
	float ZNormal;
};

class Primitive
{
public:
	Primitive(void);
	~Primitive(void);

	std::vector<Vertex*> verticies;

	int Serialize(char* pointerStart);
	int Deserialize(char* pointerStart);
};

#endif //PRIMITIVE_H