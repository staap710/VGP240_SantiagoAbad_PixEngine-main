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
        return
            "SetCameraPosition(x, y, z)"
            "\n"
            "- sets the position of the camera";
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
        return
            "SetCameraDirection(x, y, z)"
            "\n"
            "- sets the direction of the camera";
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
        return
            "SetCameraNear(value)"
            "- sets the camera near value";
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
        return
            "SetCameraFar(value)"
            "- sets the camera Far value";
    }

    bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraFOV : public Command
{
public:
    const char* GetName() override
    {
        return "SetCameraFov";
    }

    const char* GetDescription() override
    {
        return
            "SetCameraFov(radians)"
            "- sets the camera field of view angle";
    }

    bool Execute(const std::vector<std::string>& params) override;
};