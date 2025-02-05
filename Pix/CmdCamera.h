#pragma once

#include "Command.h"

class CmdSetCameraPosition : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraPosition";
	}

	const char* GetDescription() override
	{
		return "SetCameraPosition (x, y, z)"
			"\n"
			"- sets the camera position.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraDirection : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraDirection";
	}

	const char* GetDescription() override
	{
		return "SetCameraPosition (x, y, z)"
			"\n"
			"- sets the camera's direction.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraNear : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraNear";
	}

	const char* GetDescription() override
	{
		return "SetCameraNear (value)"
			"\n"
			"- sets the camera's near value/ distance.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraFar : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraFar";
	}

	const char* GetDescription() override
	{
		return "SetCameraFar (value)"
			"\n"
			"- sets the camera's far value/ distance.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraFov : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraFov";
	}

	const char* GetDescription() override
	{
		return "SetCameraFar (radians)"
			"\n"
			"- sets the camera's field of view angle.";
	}

	bool Execute(const std::vector<std::string>& params) override;
};