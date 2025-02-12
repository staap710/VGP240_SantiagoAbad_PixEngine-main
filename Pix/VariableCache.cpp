#include "VariableCache.h"

#include <ImGui/Inc/imgui.h>
#include <algorithm>

struct Variable
{
	virtual ~Variable() = default;
	virtual void ShowUI() = 0;
	std::string name;
};

struct FloatVar : public Variable
{
	void ShowUI() override
	{
		ImGui::DragFloat(name.c_str(), &value, speed, min, max);
	}

	float value = 0.0f;
	float speed = 1.0f;
	float min = 0.0f;
	float max = 1.0f;
};

struct BoolVar : public Variable
{
	void ShowUI() override
	{
		ImGui::Checkbox(name.c_str(), &value);
	}

	bool value = false;
};

VariableCache* VariableCache::Get()
{
	static VariableCache sInstance;
	return &sInstance;
}

void VariableCache::Clear()
{
	mVariables.clear();
}

bool VariableCache::IsVarName(const std::string& name) const
{
	return !name.empty() && name[0] == '$';
}

void VariableCache::AddFloat(const std::string& name, float value, float speed, float min, float max)
{
	// Add the variable if it does not already exist
	auto iter = std::find_if(mVariables.begin(), mVariables.end(), [name](auto& var)
		{
			return var->name == name;
		});
	if (iter == mVariables.end())
	{
		auto floatVar = std::make_unique<FloatVar>();
		floatVar->name = name;
		floatVar->value = value;
		floatVar->speed = speed;
		floatVar->min = min;
		floatVar->max = max;
		mVariables.emplace_back(std::move(floatVar));
	}
}

float VariableCache::GetFloat(const std::string& param)
{
	if (IsVarName(param))
	{
		auto iter = std::find_if(mVariables.begin(), mVariables.end(), [param](auto& var)
			{
				return var->name == param;
			});
		if (iter != mVariables.end())
		{
			return static_cast<FloatVar*>((*iter).get())->value;
		}
	}

	return stof(param);
}

void VariableCache::AddBool(const std::string& name, bool value)
{
	if (IsVarName(name))
	{
		auto iter = std::find_if(mVariables.begin(), mVariables.end(), [name](auto& var)
			{
				return var->name == name;
			});
		if (iter == mVariables.end())
		{
			auto boolVar = std::make_unique<BoolVar>();
			boolVar->name = name;
			boolVar->value = value;
			mVariables.emplace_back(std::move(boolVar));
		}
	}
}
bool VariableCache::GetBool(const std::string& param)
{
	if (IsVarName(param))
	{
		auto iter = std::find_if(mVariables.begin(), mVariables.end(), [param](auto& var)
			{
				return var->name == param;
			});
		if (iter != mVariables.end())
		{
			return static_cast<BoolVar*>((*iter).get())->value;
		}
	}
	return param == "true" ? true : false;
}

void VariableCache::ShowEditor()
{
	if (mVariables.empty())
		return;

	ImGui::Begin("Variables", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	for (auto& var : mVariables)
		var->ShowUI();
	ImGui::End();
}