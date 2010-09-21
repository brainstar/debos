#ifndef LINE_H
#define LINE_H

using namespace std;

class Line {
public:
	Line();
	Line(float *x, float *y);
	~Line() { };

	float* geta();
	float* getb();

	void setColor(float r, float g, float b);

	void draw(bool edit);

private:
	float a[3], b[3];
	float col[3];
};

#endif
