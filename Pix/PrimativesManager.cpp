#include "PrimativesManager.h"
#include "Rasterizer.h"
#include "Clipper.h"

#include "MatrixStack.h"
#include "Camera.h"
#include "LightManager.h"

extern float gResolutionX;
extern float gResolutionY;

namespace
{
	Matrix4 GetScreenTransform()
	{
		float hw = gResolutionX * 0.5f;
		float hh = gResolutionY * 0.5f;
		return Matrix4(
			hw, 0.0f, 0.0f, 0.0f,
			0.0f, -hh, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			hw, hh, 0.0f, 1.0f);
	}

	bool CullTriangle(CullMode mode, const std::vector<Vertex>& triangleInNDC)
	{
		if (mode == CullMode::None)
		{
			return false;
		}

		Vector3 abDir = triangleInNDC[1].pos - triangleInNDC[0].pos;
		Vector3 acDir = triangleInNDC[2].pos - triangleInNDC[0].pos;
		Vector3 faceNormNDC = MathHelper::Normalize(MathHelper::Cross(abDir, acDir));

		if (mode == CullMode::Back)
		{
			return faceNormNDC.z > 0.0f;
		}

		if (mode == CullMode::Front)
		{
			return faceNormNDC.z < 0.0f;
		}

		return false;
	}

	Vector3 CreateFaceNormal(const std::vector<Vertex>& triangle)
	{
		Vector3 abDir = triangle[1].pos - triangle[0].pos;
		Vector3 acDir = triangle[2].pos - triangle[0].pos;
		Vector3 faceNormal = MathHelper::Normalize(MathHelper::Cross(abDir, acDir));
		return faceNormal;
	}
}

PrimativesManager* PrimativesManager::Get()
{
	static PrimativesManager sInstance;
	return &sInstance;
}
PrimativesManager::PrimativesManager()
{

}

void PrimativesManager::OnNewFrame()
{
	mCullMode = CullMode::Back;
}

void PrimativesManager::SetCullMode(CullMode mode)
{
	mCullMode = mode;
}

bool PrimativesManager::BeginDraw(Topology topology, bool applyTransform)
{
	mTopology = topology;
	mApplyTransform = applyTransform;
	mDrawBegin = true;
	mVertexBuffer.clear();
	return true;
}
void PrimativesManager::AddVertex(const Vertex& vertex)
{
	if (mDrawBegin) {
		mVertexBuffer.push_back(vertex);
	}
}

bool PrimativesManager::EndDraw()
{
	if (!mDrawBegin)
	{
		return false;
	}



	switch (mTopology) {
	case Topology::Point:
	{
		for (size_t i = 0; i < mVertexBuffer.size(); ++i) {
			if (!Clipper::Get()->ClipPoint(mVertexBuffer[i]))
			{
				Rasterizer::Get()->DrawPoint(mVertexBuffer[i]);
			}

		}
	}
	break;
	case Topology::Line:
	{
		for (size_t i = 1; i < mVertexBuffer.size(); i += 2)
		{


			if (!Clipper::Get()->ClipLine(mVertexBuffer[i - 1], mVertexBuffer[i]))
			{
				Rasterizer::Get()->DrawLine(mVertexBuffer[i - 1], mVertexBuffer[i]);
			}

		}
	}
	break;
	case Topology::Triangle:
	{

		for (size_t i = 2; i < mVertexBuffer.size(); i += 3)
		{
			std::vector<Vertex> triangle = { mVertexBuffer[i - 2], mVertexBuffer[i - 1], mVertexBuffer[i] };

			if (mApplyTransform)
			{
				Matrix4 matWorld = MatrixStack::Get()->GetTransform();
				Matrix4 matView = Camera::Get()->GetViewMatrix();
				Matrix4 matProj = Camera::Get()->GetProjectionMatrix();
				Matrix4 matScreen = GetScreenTransform();
				Matrix4 matNDC = matView * matProj;
				ShadeMode shadeMode = Rasterizer::Get()->GetShadeMode();

				// transform position to world space
				for (size_t t = 0; t < triangle.size(); ++t)
				{
					triangle[t].pos = MathHelper::TransformCoord(triangle[t].pos, matWorld);
					triangle[t].posWorld = triangle[t].pos;
				}
				// if we dont have a normal, add one
				if (MathHelper::IsEqual(MathHelper::MagnitudeSquared(triangle[0].norm), 0.0f))
				{
					Vector3 faceNormal = CreateFaceNormal(triangle);
					for (size_t t = 0; t < triangle.size(); ++t)
					{
						triangle[t].norm = faceNormal;
					}
				}
				//if we have, transform into world space
				else
				{
					for (size_t t = 0; t < triangle.size(); ++t)
					{
						triangle[t].norm = MathHelper::TransformNormal(triangle[t].norm, matWorld);
					}
				}

				//apply light vertices
				// lighting needs to be calculated in world space (vertex lighting and pixel lighting)

				Vector3 faceNormal = CreateFaceNormal(triangle);
				if (shadeMode == ShadeMode::Flat)
				{
					triangle[0].color *= LightManager::Get()->ComputeLightColor(triangle[0].pos, triangle[0].norm);
					triangle[1].color = triangle[0].color;
					triangle[2].color = triangle[0].color;
				}
				else if (shadeMode == ShadeMode::Gouraud)
				{
					for (size_t t = 0; t < triangle.size(); ++t)
					{
						triangle[t].color *= LightManager::Get()->ComputeLightColor(triangle[t].pos, triangle[t].norm);
					}
				}


				for (size_t t = 0; t < triangle.size(); ++t)
				{
					triangle[t].color *= LightManager::Get()->ComputeLightColor(triangle[t].pos, faceNormal);
				}

				//transform to NDC space
				for (size_t t = 0; t < triangle.size(); ++t)
				{
					triangle[t].pos = MathHelper::TransformCoord(triangle[t].pos, matNDC);
				}

				//check if the triangle is back facing
				if (CullTriangle(mCullMode, triangle))
				{
					continue;
				}

				//tranform from NDC to screen space
				for (size_t t = 0; t < triangle.size(); ++t)
				{
					triangle[t].pos = MathHelper::TransformCoord(triangle[t].pos, matScreen);
					MathHelper::FlattenVectorScreenCoords(triangle[t].pos);
				}
			}

			if (!Clipper::Get()->ClipTriangle(triangle))
			{
				for (size_t v = 2; v < triangle.size(); ++v)
				{
					Rasterizer::Get()->DrawTriangle(triangle[0], triangle[v - 1], triangle[v]);
				}
			}

		}
	}
	break;
	default:
		break;
	}

	mDrawBegin = false;
	return true;
}