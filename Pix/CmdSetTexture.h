#pragma once

#include "Command.h"

class CmdSetTexture : public Command
{
public:
    const char* GetName() override 
    { 
        return "SetTexture"; 
    }

    const char* GetDescription() override
    {
        return "Set Texture(fileName)"
            "\n"
            "- Sets the texture to use for rendering";
    }

    bool Execute(const std::vector<std::string>& params) override;
};