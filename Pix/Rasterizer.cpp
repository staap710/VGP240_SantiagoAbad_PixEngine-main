#include "Rasterizer.h"

void DrawLineLow(const Vertex& left, const Vertex& right)
{
	float dx = right.pos.x - left.pos.x;
	int startX = static_cast<int>(left.pos.x);
	int endX = static_cast<int>(right.pos.x);
	
	for (int x = startX; x <= endX; x++)
	{
		float t = static_cast<float>(x - startX) / dx;
		Rasterizer::Get()->DrawPoint(LerpVertex(left, right, t));
	}
}

void DrawLineHigh(const Vertex& bottom, const Vertex& top)
{
	float dy = top.pos.y - bottom.pos.y;
	int startY = static_cast<int>(bottom.pos.y);
	int endY = static_cast<int>(top.pos.y);

	for (int y = startY; y <= endY; ++y)
	{
		float t = static_cast<float>(y - startY) / dy;
		Rasterizer::Get()->DrawPoint(LerpVertex(bottom, top, t));
	}
}

Rasterizer* Rasterizer::Get()
{
	static Rasterizer sInstance;
	return &sInstance;
}

void Rasterizer::SetColor(X::Color color)
{
	mColor = color;
}

void Rasterizer::SetFillMode(FillMode fillMode)
{
	mFillMode = fillMode;
}

void Rasterizer::DrawPoint(int x, int y)
{
	X::DrawPixel(x, y, mColor);
}

void Rasterizer::DrawPoint(const Vertex& vertex)
{
	SetColor(vertex.color);
	DrawPoint(static_cast<int>(vertex.pos.x), static_cast<int>(vertex.pos.y));
}

void Rasterizer::DrawLine(const Vertex& a, const Vertex& b)
{
	float dx = b.pos.x - a.pos.x;
	float dy = b.pos.y - a.pos.y;

	if (MathHelper::IsEqual(dx, 0.0f))
	{
		if (a.pos.y < b.pos.y)
		{
			DrawLineHigh(a, b);
		}
		else
		{
			DrawLineHigh(b, a);
		}
		return;
	}

	float m = dy / dx;
	if (abs(m) < 1)
	{
		if (a.pos.x < b.pos.x)
		{
			DrawLineLow(a, b);
		}
		else
		{
			DrawLineLow(b, a);
		}
	}
	else
	{
		if (a.pos.y < b.pos.y)
		{
			DrawLineHigh(a, b);
		}
		else
		{
			DrawLineHigh(b, a);
		}
	}
}

void Rasterizer::DrawTriangle(const Vertex& a, const Vertex& b, const Vertex& c)
{
	switch (mFillMode)
	{
	case FillMode::Wireframe:
	{
		DrawLine(a, b);
		DrawLine(b, c);
		DrawLine(c, a);
	}
	break;
	case FillMode::Solid:
	{
		std::vector<Vertex> sortedVertices = { a, b, c };
		std::sort(sortedVertices.begin(), sortedVertices.end(),
			[](const Vertex& lhs, const Vertex& rhs)
			{
				return lhs.pos.y < rhs.pos.y;
			});
		DrawFilledTriangle(sortedVertices[0], sortedVertices[1], sortedVertices[2]);
	}
	break;
	default:
		break;
	}
}

void Rasterizer::DrawFilledTriangle(const Vertex& a, const Vertex& b, const Vertex& c)
{
	float dy = c.pos.y - a.pos.y;

	if (MathHelper::IsEqual(a.pos.y, b.pos.y))
	{
		int startY = static_cast<int>(a.pos.y);
		int endY = static_cast<int>(c.pos.y);

		for (int y = startY; y <= endY; ++y)
		{
			float t = static_cast<float>(y - startY) / dy;
			Vertex aSide = LerpVertex(a, c, t);
			Vertex bSide = LerpVertex(b, c, t);
			DrawLine(aSide, bSide);
		}
	}

	else if (MathHelper::IsEqual(b.pos.y, c.pos.y))
	{
		int startY = static_cast<int>(a.pos.y);
		int endY = static_cast<int>(c.pos.y);

		for (int y = startY; y <= endY; ++y)
		{
			float t = static_cast<float>(y - startY) / dy;
			Vertex aSide = LerpVertex(a, b, t);
			Vertex bSide = LerpVertex(a, c, t);
			DrawLine(aSide, bSide);
		}
	}

	else
	{
		float t = (b.pos.y - a.pos.y) / dy;
		Vertex splitVertex = LerpVertex(a, c, t);
		DrawFilledTriangle(a, b, splitVertex);
		DrawFilledTriangle(b, splitVertex, c);
	}
}

//void Rasterizer::DrawTriangle(const Vertex& a, const Vertex& b, const Vertex& c)
//{
//	switch (mFillmode)
//	{
//	case FillMode::Wireframe:
//	{
//
//	}
//	break;
//
//	case FillMode::Solid:
//	{
//
//	}
//	break;
//
//	}
//	DrawLine(a, b);
//	DrawLine(b, c);
//	DrawLine(c, a);
//}