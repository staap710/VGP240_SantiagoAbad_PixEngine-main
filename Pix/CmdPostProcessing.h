#pragma once
#include "Command.h"

class CmdPostProcessing : public Command
{
public:
	const char* GetName() override
	{
		return "PostProcessingBeginDraw";
	}

	const char* GetDescription() override
	{
		return
			"PostProcessingBeginDraw()"
			"\n"
			"- initializes rendering to the render target instead of the main screen";
	}
	bool Execute(const std::vector<std::string>& params) override;

};

class CmdPostProcessingEndDraw : public Command
{
public:
	const char* GetName() override
	{
		return "PostProcessingEndDraw";
	}

	const char* GetDescription() override
	{
		return
			"PostProcessing(effectType)"
			"\n"
			"- sets the effect to apply to the image"
			"- none, monochrome, inver, mirror, blur"
			"colorMask"
			"- finalizes rendering to the render target and draws the final image to the main screen";
	}
	bool Execute(const std::vector<std::string>& params) override;	

};