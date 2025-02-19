#include "LightTypes.h"
#include "Camera.h"
#include "Camera.h"

x::Color DirectionalLight::ComputeLightColor(const Vector3& position, const Vector3& normal)
{
	Camera* camera = Camera::Get();
	MaterialManager* mm = MaterialManager::Get();

	// calculate ambient color
	X::Color colorAmbient = mAmbient * mm->GetMaterialAmbient();

	// calculate diffuse color
	// directional light iL = 1.0f; (light intensity)
	Vector3 L = -mDirection;
	float dot = MathHelper::Dot(L, normal);
	X::Color colorDiffuse = mDiffuse * mm->GetMaterialDiffuse() * dot;

	//calculate specular color
	// directional light iL = 1.0f; (light intensity)
	Vector3 v = MathHelper::Normalize(camera->GetPosition() - position);
	Vector3 h = MathHelper::Normalize(v + L);
	float fallOff = X::Math::Max((float)pow(MathHelper::Dot(r, normal), mm-> GetMaterialShininess));
	X::Color colorSpecular= mSpecular * mm->GetMaterialSpecular()* fallOff;
	return colorAmbient + colorDiffuse + colorSpecular;
}

void DirectionalLight::SetDirection(const Vector3& direction)
{
	mDirection = MathHelper::Normalize(direction);
}

// PointLight
X::Color PointLight::ComputeLightColor(const Vector3& position, const Vector3& normal)
{
	Camera* camera = Camera::Get();
	MaterialManager* mm = MaterialManager::Get();

	// calculate ambient color
	X::Color colorAmbient = mAmbient * mm->GetMaterialAmbient();

	// calculate diffuse color
	Vector3 L = MathHelper::Normalize(mPosition - position);
	float dot = MathHelper::Dot(L, normal);
	X::Color colorDiffuse = mDiffuse * mm->GetMaterialDiffuse() * dot;

	// calculate specular color
	Vector3 v = MathHelper::Normalize(camera->GetPosition() - position);
	Vector3 h = MathHelper::Normalize(v + L);
	float fallOff = X::Math::Max((float)pow(MathHelper::Dot(h, normal), mm->GetMaterialShininess()));
	X::Color colorSpecular = mSpecular * mm->GetMaterialSpecular() * fallOff;
	return colorAmbient + colorDiffuse + colorSpecular;
}