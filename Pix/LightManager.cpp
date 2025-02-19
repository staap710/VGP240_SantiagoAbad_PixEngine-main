#include "LightManager.h"
#include "MaterialManager.h"
#include "LightTypes.h"

LightManager* LightManager::Get()
{
	static LightManager sInstance;
	return &sInstance;
}

void LightManager::OnNewFrame()
{
	mLights.clear();
	mAmbient = X::Colors::White;
	mDiffuse = X::Colors::White;
	mSpecular = X::Colors::White;
}

void LightManager::SetLightAmbient(const X::Color& color)
{
	mAmbient = color;
}
void LightManager::SetLightDiffuse(const X::Color& color)
{
	mDiffuse = color;
}
void LightManager::SetLightSpecular(const X::Color& color)
{
	mSpecular = color;
}

void LightManager::AddDirectionalLight(const Vector3& direction)
{
	auto light = std::make_unique<DirectionalLight>();
	light->SetAmbient(mAmbient);
	light->SetDiffuse(mDiffuse);
	light->SetSpecular(mSpecular);
	light->SetDirection(direction);
	mLights.emplace_back(std::move(light));
}

X::Color LightManager::ComputeLightColor(const Vector3& position, const Vector3& normal)
{
	X::Color color = MaterialManager::Get()->GetMaterialEmissive();
	for (auto& light : mLights)
	{
		color += light->ComputeLightColor(position, normal);
	}
	return color;
}