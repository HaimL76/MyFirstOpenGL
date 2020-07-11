#include <vector>
#include "DrawingObject.h"
#include "LineObject.h"

using namespace std;

class MyOpenGL {
public:
	void Draw();
	void Render();
	void Add(DrawingObject* obj);

private:
	vector<DrawingObject*> vec;
};