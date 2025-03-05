#include "Rasterizer.h"
#include "DepthBuffer.h"
#include "LightManager.h"

void DrawLineLow(const Vertex& left, const Vertex& right, ShadeMode shadeMode)
{
	float dx = right.pos.x - left.pos.x;
	int startX = static_cast<int>(left.pos.x);
	int endX = static_cast<int>(right.pos.x);
	for (int x = startX; x <= endX; ++x)
	{
		float t = static_cast<float>(x - startX) / dx;
		Rasterizer::Get()->DrawPoint(LerpVertex(left, right, t, shadeMode == ShadeMode::Phong));
	}
}

void DrawLineHigh(const Vertex& bottom, const Vertex& top, ShadeMode shadeMode)
{
	float dy = top.pos.y - bottom.pos.y;
	int startY = static_cast<int>(bottom.pos.y);
	int endY = static_cast<int>(top.pos.y);
	for (int y = startY; y <= endY; ++y)
	{
		float t = static_cast<float>(y - startY) / dy;
		Rasterizer::Get()->DrawPoint(LerpVertex(bottom, top, t, shadeMode == ShadeMode::Phong));
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

void Rasterizer::SetShadeMode(ShadeMode shadeMode)
{
	mShadeMode = shadeMode;
}

ShadeMode Rasterizer::GetShadeMode()
{
	return mShadeMode;
}

void Rasterizer::DrawPoint(int x, int y)
{
	X::DrawPixel(x, y, mColor);
}

void Rasterizer::DrawPoint(const Vertex& vertex)
{
	int x = static_cast<int>(vertex.pos.x);
	int y = static_cast<int>(vertex.pos.y);
	if (DepthBuffer::Get()->CheckDepthBuffer(x, y, vertex.pos.z))
	{
		X::Color color = vertex.color;
		if (mShadeMode == ShadeMode::Phong)
		{
			color *= LightManager::Get()->ComputeLightColor(vertex.posWorld, vertex.norm);
		}
		SetColor(color);
		DrawPoint(x, y);
	}
}

void Rasterizer::DrawLine(const Vertex& a, const Vertex& b)
{
	float dx = b.pos.x - a.pos.x;
	float dy = b.pos.y - a.pos.y;
	// if the dx is 0, we are drawing straight up(vertical line)
	if (MathHelper::IsEqual(dx, 0.0f))
	{
		if (a.pos.y < b.pos.y)
		{
			DrawLineHigh(a, b, mShadeMode);
		}
		else
		{
			DrawLineHigh(b, a, mShadeMode);
		}
		return;
	}


	float m = dy / dx;
	if (abs(m) < 1)
	{
		if (a.pos.x < b.pos.x)
		{
			DrawLineLow(a, b, mShadeMode);
		}
		else
		{
			DrawLineLow(b, a, mShadeMode);
		}
	}
	else
	{
		if (a.pos.y < b.pos.y)
		{
			DrawLineHigh(a, b, mShadeMode);
		}
		else
		{
			DrawLineHigh(b, a, mShadeMode);
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
		std::vector<Vertex> sortedVertices{ a, b, c };
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
	// if a and b match meaning the triangle has a flat bottom
	if (MathHelper::IsEqual(a.pos.y, b.pos.y))
	{
		int startY = static_cast<int>(a.pos.y);
		int endY = static_cast<int>(c.pos.y);
		for (int y = startY; y <= endY; ++y)
		{
			float t = static_cast<float>(y - startY) / dy;
			Vertex aSide = LerpVertex(a, c, t, mShadeMode == ShadeMode::Phong);
			Vertex bSide = LerpVertex(b, c, t, mShadeMode == ShadeMode::Phong);
			DrawLine(aSide, bSide);
		}
	}
	// if b and c match the triangle has a flat top
	else if (MathHelper::IsEqual(b.pos.y, c.pos.y))
	{
		int startY = static_cast<int>(a.pos.y);
		int endY = static_cast<int>(c.pos.y);
		for (int y = startY; y <= endY; ++y)
		{
			float t = static_cast<float>(y - startY) / dy;
			Vertex bSide = LerpVertex(a, b, t, mShadeMode == ShadeMode::Phong);
			Vertex cSide = LerpVertex(a, c, t, mShadeMode == ShadeMode::Phong);
			DrawLine(bSide, cSide);
		}
	}
	// there are no flat top/bottom so split triangle
	else {
		float t = (b.pos.y - a.pos.y) / dy;
		Vertex splitVertex = LerpVertex(a, c, t, mShadeMode == ShadeMode::Phong);
		// top filled
		DrawFilledTriangle(a, b, splitVertex);
		// bottom filled
		DrawFilledTriangle(b, splitVertex, c);
	}

}