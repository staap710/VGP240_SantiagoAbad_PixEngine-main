#include "CmdEndDraw.h"
#include "PrimativesManager.h"

bool CmdEndDraw::Execute(const std::vector<std::string>& params)
{
	return PrimativesManager::Get()->EndDraw();
}