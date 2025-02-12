#pragma once

#include "Command.h"

class CmdSetCullMode : public Command
{
public:
    const char* GetName() override
    {
        return "SetCullMode";
    }

    const char* GetDescription() override
    {
        return
            "SetCullMode(mode)\n"
            "\n"
            "- none will not use any face culling"
            "- back will not anything facing away form the camera"
            "- front will not render anything facing towards the camera";
    }

    bool Execute(const std::vector<std::string>& params) override;
};