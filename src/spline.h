#ifndef SPLINE_H
#define SPLINE_H

class Spline {
public:
	Spline();
	Spline(float *a, float *k1, float *k2, float *b);

	float* geta() { return &(points[0][0]); }
	float* getk1() { return &(points[1][0]); }
	float* getk2() { return &(points[2][0]); }
	float* getb() { return &(points[3][0]); }

	void seta(float *p);
	void setk1(float *p);
	void setk2(float *p);
	void setb(float *p);

	void draw();

private:
	float points[4][3];
};

#endif
