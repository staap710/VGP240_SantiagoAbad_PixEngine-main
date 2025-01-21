#pragma once

#include "Vertex.h"

enum class Topology // 'class' -> Topology::Line
{
	Point,
	Line,
	Triangle
};

class PrimativesManager
{
public:

	static PrimativesManager* Get(); // Singleton

	bool BeginDraw(Topology topology);

	void AddVertex(const Vertex& vertex); // So that a copy isnt made everytime (duplicates chopping memory)

	bool EndDraw();

private:

	PrimativesManager(); // So it can only be called by itself

	std::vector<Vertex> mVertexBuffer;

	Topology mTopology = Topology::Point;
	bool mDrawBegin = false; 
};