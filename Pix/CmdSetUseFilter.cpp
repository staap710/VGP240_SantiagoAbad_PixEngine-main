#include "CmdSetUseFilter.h"
#include "TextureManager.h"
#include "VariableCache.h"

bool CmdSetUseFilter::Execute(const std::vector<std::string>& params)
{
    if (params.size() < 1)
    {
        return false;
    }

    bool useFilter = VariableCache::Get()->GetBool(params[0]);

    TextureManager::Get()->SetUseFilter(useFilter);
    return true;
}