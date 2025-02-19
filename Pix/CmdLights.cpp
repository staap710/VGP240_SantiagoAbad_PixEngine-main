#include "CmdLights.h"
#include "LightManager.h"
#include "VariableCache.h"

bool CmdSetLightAmbient::Execute(const std::vector<std::string>& params)
{
    if (params.size() < 3)
    {
        return false;
    }

    VariableCache* vc = VariableCache::Get();

    const float r = vc->GetFloat(params[0]);
    const float g = vc->GetFloat(params[1]);
    const float b = vc->GetFloat(params[2]);

    LightManager::Get()->SetLightAmbient({ r, g, b, 1.0f });
    return true;
}

bool CmdSetLightDiffuse::Execute(const std::vector<std::string>& params)
{
    if (params.size() < 3)
    {
        return false;
    }

    VariableCache* vc = VariableCache::Get();

    const float r = vc->GetFloat(params[0]);
    const float g = vc->GetFloat(params[1]);
    const float b = vc->GetFloat(params[2]);

    LightManager::Get()->SetLightDiffuse({ r, g, b, 1.0f });
    return true;
}

bool CmdSetLightSpecular::Execute(const std::vector<std::string>& params)
{
    if (params.size() < 3)
    {
        return false;
    }

    VariableCache* vc = VariableCache::Get();

    const float r = vc->GetFloat(params[0]);
    const float g = vc->GetFloat(params[1]);
    const float b = vc->GetFloat(params[2]);

    LightManager::Get()->SetLightSpecular({ r, g, b, 1.0f });
    return true;
}

bool CmdAddDirectionalLight::Execute(const std::vector<std::string>& params)
{
    if (params.size() < 3)
    {
        return false;
    }

    VariableCache* vc = VariableCache::Get();

    const float x = vc->GetFloat(params[0]);
    const float y = vc->GetFloat(params[1]);
    const float z = vc->GetFloat(params[2]);

    LightManager::Get()->AddDirectionalLight({ x, y, z });
    return true;
}

bool CmdAddPointLight::Execute(const std::vector<std::string>& params)
{
    if (params.size() < 3)
    {
        return false;
    }

    VariableCache* vc = VariableCache::Get();
    const float x = vc->GetFloat(params[0]);
    const float y = vc->GetFloat(params[1]);
    const float z = vc->GetFloat(params[2]);
    float constant = 1.0f;
    float linear = 0.0f;
    float quadratic = 0.0f;

    if (params.size() >= 4)
    {
        constant = vc->GetFloat(params[3]);
    }
}