#include "bezier.h"
#include <math.h>
#include <Qt/qwidget.h>
#define PI 3.1415926
using namespace std;

BezierPoint::BezierPoint(float *a, float *b, float *c) {
	for (int i = 0; i < 3; i++) {
		p[i] = a[i];
		pl[i] = b[i];
		pr[i] = c[i];
	}
}

BezierPoint::BezierPoint() {
	for (int i = 0; i < 3; i++)
		p[i] = pl[i] = pr[i] = 0.0;
}

float* BezierPoint::getp() {
	return p;
}

float* BezierPoint::getpl() {
	return pl;
}

float* BezierPoint::getpr() {
	return pr;
}

void BezierPoint::setp(float *a) {
	float diff[3];
	for (int i = 0; i < 3; i++) {
		diff[i] = *(a+i) - p[i];
		p[i] = *(a+i);
		pl[i] += diff[i];
		pr[i] += diff[i];
	}
}

void BezierPoint::setpl(float *a) {
	float x = a[0] - p[0], y = a[1] - p[1];
	float l = sqrtf(x*x + y*y);
	float phi;
	
	if (l == 0.0) return;
	
	if (y >= 0.0)
		phi = acosf(x / l);
	else
		phi = -acosf(x / l);	

	phi += PI;
	
	x = pr[0] - p[0]; y = pr[1] - p[1];
	l = sqrtf(x*x + y*y);
	
	pr[0] = p[0] + l * cosf(phi);
	pr[1] = p[1] + l * sinf(phi);
	
	for (int i = 0; i < 3; i++)
		pl[i] = a[i];
}

void BezierPoint::setpr(float *a) {
	float x = a[0] - p[0], y = a[1] - p[1];
	float l = sqrtf(x*x + y*y);
	float phi;
	
	if (l == 0.0) return;
	
	if (y >= 0.0)
		phi = acosf(x / l);
	else
		phi = -acosf(x / l);	

	phi += PI;
	
	x = pl[0] - p[0]; y = pl[1] - p[1];
	l = sqrtf(x*x + y*y);
	
	pl[0] = p[0] + l * cosf(phi);
	pl[1] = p[1] + l * sinf(phi);
	
	for (int i = 0; i < 3; i++)
		pr[i] = a[i];
}

float BezierPoint::getAngle(float x, float y) {
	float a, b, c, angle;
	
	a = x - p[0];
	b = y - p[1];
	c = sqrtf(a*a + b*b);
	
	angle = acosf(a / c) * 180. / PI;
	if (b / c > 0.) {
		angle = 0. - angle;
	}
	
	return angle;
}

void BezierPoint::turn(float degree) {
	float x = pr[0] - p[0], y = pr[1] - p[1];
	float l = sqrtf(x*x + y*y);
	float phi;
	bool left = false;
	
	if (l == 0.0) {
		x = pr[0] - p[0], y = pr[1] - p[1];
		l = sqrtf(x*x + y*y);
		if (l == 0.0)
			return;
		left = true;
	}
	
	if (y >= 0.0)
		phi = acosf(x / l);
	else
		phi = -acosf(x / l);
	phi -= (degree / 180. * PI);
	
	x = l * cosf(phi);
	y = l * sinf(phi);
	
	float a[3];
	a[0] = p[0] + x;
	a[1] = p[1] + y;
	a[2] = p[2];
	
	if (!left)
		setpr(a);
	else
		setpl(a);
}

void BezierPoint::move(float x, float y) {
	float a[3];
	a[0] = p[0] + x;
	a[1] = p[1] + y;
	a[2] = p[2];
	
	setp(a);
}
