#pragma once
#include "Vertex.h"

class Clipper {
public:
	static Clipper* Get();
	void OnNewFrame();
	bool ClipPoint(const Vertex& v);
	bool ClipLine(Vertex& a, Vertex& b);
	bool IsClipping()const;
	void SetClipping(bool clip);
	bool ClipTriangle(std::vector<Vertex>& vertices);
private:
	bool mClipping = false;
};
