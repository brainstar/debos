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

	virtual bool addPoint(float x, float y, float z) {};
	virtual void nextInstance() {};
	virtual void prevInstance() {};
	virtual void addInstance(float *a, float *k1, float *k2, float *b) {};
	virtual void addInstance(float *a, float *b) {};
	virtual void deleteInstance() {};

	virtual void iterToEnd() {};
	
	OBJECT_TYPE type;
	float points[4][3];
	int pCount;
};

#endif
