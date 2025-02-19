#pragma once

#include "Command.h"

class CmdSetMaterialEmissive : public Command
{
public:
	const char* GetName() override
	{
		return "SetMaterialEmissive";
	}

	const char* GetDescription() override
	{
		return "SetMaterialEmissive(r, g, b)\n"
			"\n"
			"- Sets the Emissive Color for a material.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetMaterialAmbient : public Command
{
public:
	const char* GetName() override
	{
		return "SetMaterialAmbient";
	}

	const char* GetDescription() override
	{
		return "SetMaterialAmbient(r, g, b)\n"
			"\n"
			"- Sets the Ambient Color for a material.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetMaterialDiffuse : public Command
{
public:
	const char* GetName() override
	{
		return "SetMateriaDiffuse";
	}

	const char* GetDescription() override
	{
		return "SetMaterialDiffuse(r, g, b)\n"
			"\n"
			"- Sets the Diffuse Color for a material.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetMaterialSpecular : public Command
{
public:
	const char* GetName() override
	{
		return "SetMaterialSpecular";
	}

	const char* GetDescription() override
	{
		return "SetMaterialSpecular(r, g, b)\n"
			"\n"
			"- Sets the Specular Color for a material.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetMaterialShininess : public Command
{
public:
	const char* GetName() override
	{
		return "SetMaterialShininess";
	}

	const char* GetDescription() override
	{
		return "SetMaterialshininess(shininess)\n"
			"\n"
			"- Sets the Shininess Value for a material.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};
