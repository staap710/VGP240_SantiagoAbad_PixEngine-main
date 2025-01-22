#include "Clipper.h"
#include "Viewport.h"

const short BIT_INSIDE = 0;     // 0000
const short BIT_LEFT = 1 << 1;  // 0001
const short BIT_RIGHT = 1 << 2; // 0010
const short BIT_BOTTOM = 1 << 3;// 0100
const short BIT_TOP = 1 << 4;   // 1000

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
	case ClipEdge::Left:
		return pos.x > vp->GetMinX();
	case ClipEdge::Top:
		return pos.y > vp->GetMinY();
	case ClipEdge::Right:
		return pos.x < vp->GetMaxX();
	case ClipEdge::Bottom:
		return pos.y < vp->GetMaxY();
	default:
		break;
	}
	return false;
}

Vertex ComputeIntersection(ClipEdge edge, const Vertex& vN, const Vertex& vNP1)
{
	Viewport* vp = Viewport::Get();
	float t = 0.0f;
	switch (edge)
	{
	case ClipEdge::Left:   t = (vp->GetMinX() - vN.pos.x) / (vNP1.pos.x - vN.pos.x); break;
	case ClipEdge::Top:    t = (vp->GetMinY() - vN.pos.y) / (vNP1.pos.y - vN.pos.y); break;
	case ClipEdge::Right:  t = (vp->GetMaxX() - vN.pos.x) / (vNP1.pos.x - vN.pos.x); break;
	case ClipEdge::Bottom: t = (vp->GetMaxY() - vN.pos.y) / (vNP1.pos.y - vN.pos.y); break;
	default: break;
	}

	return LerpVertex(vN, vNP1, t);
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
	float maxX = Viewport::Get()->GetMaxX();
	float minY = Viewport::Get()->GetMinY();
	float maxY = Viewport::Get()->GetMaxY();
	return v.pos.x < minX || v.pos.x > maxX
		|| v.pos.y < minY || v.pos.y > maxY;
}

bool Clipper::ClipLine(Vertex& a, Vertex& b)
{
	if (!mClipping)
	{
		return false;
	}

	float minX = Viewport::Get()->GetMinX();
	float maxX = Viewport::Get()->GetMaxX();
	float minY = Viewport::Get()->GetMinY();
	float maxY = Viewport::Get()->GetMaxY();

	short codeA = GetOutputCode(a.pos.x, a.pos.y);
	short codeB = GetOutputCode(b.pos.x, b.pos.y);
	bool cullLine = true;

	while (true)
	{
		if (!(codeA | codeB))
		{
			// if both a and b are 0000, then draw the line
			cullLine = false;
			break;
		}
		else if (codeA & codeB)
		{
			//both values are not within the viewport so we cull the line
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
		else if (outCode & BIT_LEFT)
		{
			t = (minX - a.pos.x) / (b.pos.x - a.pos.x);
		}
		else if (outCode & BIT_RIGHT)
		{
			t = (maxX - a.pos.x) / (b.pos.x - a.pos.x);
		}

		if (outCode == codeA)
		{
			a = LerpVertex(a, b, t);
			codeA = GetOutputCode(a.pos.x, a.pos.y);
		}
		else
		{
			b = LerpVertex(a, b, t);
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

	std::vector<Vertex> newVertices;
	for (short i = 0; i < (int)ClipEdge::Count; ++i)
	{
		newVertices.clear();
		ClipEdge edge = (ClipEdge)i;

		for (size_t n = 0; n < vertices.size(); ++n)
		{
			size_t np1 = (n + 1) % vertices.size();
			const Vertex& vN = vertices[n];
			const Vertex& vNP1 = vertices[np1];

			bool nIsInFront = IsInFront(edge, vN.pos);
			bool np1IsInFront = IsInFront(edge, vNP1.pos);

			//case 1, both are in front
			if (nIsInFront && np1IsInFront)
			{
				//save np1
				newVertices.push_back(vNP1);
			}
			//case 2, both are behind
			else if (!nIsInFront && !np1IsInFront)
			{
				//save nothing
			}
			//case 3, nV is in front and vNP1 is behind
			else if (nIsInFront && !np1IsInFront)
			{
				//save intersection
				newVertices.push_back(ComputeIntersection(edge, vN, vNP1));
			}
			//case 4, nV is behind and vNP1 is in front
			else if (!nIsInFront && np1IsInFront)
			{
				//save intersection and vNP1
				newVertices.push_back(ComputeIntersection(edge, vN, vNP1));
				newVertices.push_back(vNP1);
			}
		}
		vertices = newVertices;
	}

	return newVertices.empty();
}

bool Clipper::IsClipping() const
{
	return mClipping;
}

void Clipper::SetClipping(bool clip)
{
	mClipping = clip;
}
