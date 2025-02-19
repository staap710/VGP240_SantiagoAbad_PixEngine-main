#pragma once

#include "Command.h"

class CmdSetlightAmbient : public Command {
public:
	const char* GetName()
	{
		return "SetLightAmbient";
	}
	const char* GetDescription()
	{
		return
			"SetLightAmbient(r, g, b)\n"
			"\n"
			"- sets ambient color for a light";
	}
	bool Execute(const std::vector<std::string>& params)override;
};

class CmdSetLightSpecular : public Command {
public:
	const char* GetName()
	{
		return "SetLightSpecular";
	}
	const char* GetDescription()
	{
		return
			"SetLightSpecular(r, g, b)\n"
			"\n"
			"- set the specular color for a light";
	}
	bool Execute(const std::vector<std::string>& params)override;
};

class CmdSetDirectionalLight : public Command {
public:
	const char* GetName() override
	{
		return "AddDirectionaLight";
	}
	const char* GetDescription()
	{
		return
			"Add directionaLight(r, g, b)\n"
			"\n"
			"- sets ambient color for a directional light";
	}
	bool Execute(const std::vector<std::string>& params)override;
};