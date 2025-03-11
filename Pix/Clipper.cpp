#include "Clipper.h"
#include "Viewport.h"
#include "Rasterizer.h"

const short BIT_INSIDE = 0;      // 0000
const short BIT_LEFT = 1 << 1; // 0001
const short BIT_RIGHT = 1 << 2; // 0010
const short BIT_BOTTOM = 1 << 3; // 0100
const short BIT_TOP = 1 << 4; // 1000

enum class ClipEdge : short
{
	Left,
	Top,
	Right,
	Bottom,
	Count
};

bool IsInFront(ClipEdge edge, const Vector3& pos)
{
	Viewport* vp = Viewport::Get();
	switch (edge)
	{
	case ClipEdge::Left: { return pos.x > vp->GetMinX(); }

	case ClipEdge::Top: { return pos.y > vp->GetMinY(); }

	case ClipEdge::Right: { return pos.x < vp->GetMaxX(); }

	case ClipEdge::Bottom: { return pos.y < vp->GetMaxY(); }

	default:
		break;
	}

	return false;
}

Vertex ComputeIntersection(ClipEdge edge, const Vertex& vN, const Vertex& vNP1, bool lerpNormal)
{
	Viewport* vp = Viewport::Get();
	float t = 0.0f;

	switch (edge)
	{
	case ClipEdge::Left:   t = (vp->GetMinX() - vN.pos.x) / (vNP1.pos.x - vN.pos.x); break;

	case ClipEdge::Top:    t = (vp->GetMinY() - vN.pos.y) / (vNP1.pos.y - vN.pos.y); break;

	case ClipEdge::Right:  t = (vp->GetMaxX() - vN.pos.x) / (vNP1.pos.x - vN.pos.x); break;

	case ClipEdge::Bottom: t = (vp->GetMaxY() - vN.pos.y) / (vNP1.pos.y - vN.pos.y); break;

	default:
		break;
	}

	return LerpVertex(vN, vNP1, t, lerpNormal);
}

short GetOutputCode(float x, float y)
{
	short code = BIT_INSIDE;

	Viewport* vp = Viewport::Get();

	if (x < vp->GetMinX())
	{
		code |= BIT_LEFT;
	}
	else if (x > vp->GetMaxX())
	{
		code |= BIT_RIGHT;
	}
	if (y < vp->GetMinY())
	{
		code |= BIT_TOP;
	}
	else if (y > vp->GetMaxY())
	{
		code |= BIT_BOTTOM;
	}
	return code;
}

Clipper* Clipper::Get()
{
	static Clipper sInstance;
	return &sInstance;
}

void Clipper::OnNewFrame()
{
	mClipping = false;
}

bool Clipper::ClipPoint(const Vertex& v)
{
	if (!mClipping)
	{
		return false;
	}

	float minX = Viewport::Get()->GetMinX();
	float minY = Viewport::Get()->GetMinY();
	float maxX = Viewport::Get()->GetMaxX();
	float maxY = Viewport::Get()->GetMaxY();

	return v.pos.x < minX || v.pos.x > maxX ||
		v.pos.y < minY || v.pos.y > maxY;
}

bool Clipper::ClipLine(Vertex& a, Vertex& b)
{
	if (!mClipping)
	{
		return false;
	}

	bool lerpNormal = Rasterizer::Get()->GetShadeMode() == ShadeMode::Phong;

	float minX = Viewport::Get()->GetMinX();
	float minY = Viewport::Get()->GetMinY();
	float maxX = Viewport::Get()->GetMaxX();
	float maxY = Viewport::Get()->GetMaxY();

	short codeA = GetOutputCode(a.pos.x, a.pos.y);
	short codeB = GetOutputCode(b.pos.x, b.pos.y);

	bool cullLine = true;
	while (true)
	{
		if (!(codeA | codeB)) // If both a & b are 0000 = Draw line
		{
			cullLine = false;
			break;
		}
		else if (codeA & codeB) // If both are outside viewport = Cull = Dont Draw 
		{
			break;
		}

		float t = 0.0f;
		short outCode = codeB > codeA ? codeB : codeA;
		if (outCode & BIT_TOP)
		{
			t = (minY - a.pos.y) / (b.pos.y - a.pos.y);
		}
		else if (outCode & BIT_BOTTOM)
		{
			t = (maxY - a.pos.y) / (b.pos.y - a.pos.y);
		}
		else if (outCode & BIT_RIGHT)
		{
			t = (maxX - a.pos.x) / (b.pos.x - a.pos.x);
		}
		else if (outCode & BIT_LEFT)
		{
			t = (minX - a.pos.x) / (b.pos.x - a.pos.x);
		}

		if (outCode == codeA)
		{
			a = LerpVertex(a, b, t, lerpNormal);
			codeA = GetOutputCode(a.pos.x, a.pos.y);
		}
		else
		{
			b = LerpVertex(a, b, t, lerpNormal);
			codeB = GetOutputCode(b.pos.x, b.pos.y);
		}
	}

	return cullLine;
}

bool Clipper::ClipTriangle(std::vector<Vertex>& vertices)
{
	if (!mClipping)
	{
		return false;
	}

	bool lerpNormal = Rasterizer::Get()->GetShadeMode() == ShadeMode::Phong;

	std::vector<Vertex> newVertices;

	for (int i = 0; i < (int)ClipEdge::Count; i++)
	{
		newVertices.clear();

		ClipEdge edge = (ClipEdge)i;

		for (size_t n = 0; n < vertices.size(); n++)
		{
			size_t np1 = (n + 1) % vertices.size();
			const Vertex& vN = vertices[n];
			const Vertex& vNP1 = vertices[np1];

			bool nIsInFront = IsInFront(edge, vN.pos);
			bool np1IsInFront = IsInFront(edge, vNP1.pos);

			// Case I: Both in front
			if (nIsInFront && np1IsInFront)
			{
				newVertices.push_back(vNP1);
			}
			// Case II: Both are behind
			else if (!nIsInFront && !np1IsInFront)
			{
				// Save nothing
			}
			// Case III: vN is in front, vNP1 is behind
			else if (nIsInFront && !np1IsInFront)
			{
				newVertices.push_back(ComputeIntersection(edge, vN, vNP1, lerpNormal));
			}
			// Case IV: vN is behind, VNP1 is in front
			else if (!nIsInFront && np1IsInFront)
			{
				newVertices.push_back(ComputeIntersection(edge, vN, vNP1, lerpNormal));
				newVertices.push_back(vNP1);
			}
		}

		vertices = newVertices;
	}

	return false;
}

bool Clipper::IsClipping() const
{
	return mClipping;
}

void Clipper::SetClipping(bool clip)
{
	mClipping = clip;
}