#pragma once

#include "Vertex.h"
#
enum class Topology // 'class' -> Topology::Line
{
	Point,
	Line,
	Triangle
};

enum class CullMode {
	None,
	Back,
	Front
};


class PrimativesManager
{
public:

	static PrimativesManager* Get(); // Singleton

	void OnNewFrame();
	void SetCullMode(CullMode mode);

	bool BeginDraw(Topology topology, bool applyTransform = false);

	void AddVertex(const Vertex& vertex); // So that a copy isnt made everytime (duplicates chopping memory)

	bool EndDraw();

private:

	PrimativesManager(); // So it can only be called by itself

	std::vector<Vertex> mVertexBuffer;


	Topology mTopology = Topology::Point;
	CullMode mCullMode = CullMode::Back;
	bool mDrawBegin = false;
	bool mApplyTransform = false;
};