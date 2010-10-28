#ifndef BEZIER_H
#define BEZIER_H

using namespace std;

class BezierPoint {
public:
	BezierPoint(float *a, float *b, float *c);
	BezierPoint();

	float* getp();
	float* getpl();
	float* getpr();
	
	void setp(float *a);
	void setpl(float *a);
	void setpr(float *a);
	
	float getAngle(float x, float y);
	float getLength(float x, float y);
	
	void turn(float degree);
	void move(float x, float y);
	void scale(float factor);
	void save();
	void recover();

private:
	float p[3], pl[3], pr[3], old[6];
};

#endif
