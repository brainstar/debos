#ifndef OBJECT_H
#define OBJECT_H

using namespace std;

enum OBJECT_TYPE {
	SPLINE,
	LINE
};

class Object {
public:
	virtual void draw(bool edit) {};
	
	virtual bool nextInstance() {};
	virtual bool prevInstance() {};
	virtual void deleteInstance() {};

	virtual bool addPoint(float x, float y, float z) {};
	
	OBJECT_TYPE type;
};

#endif
