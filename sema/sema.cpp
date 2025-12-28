#pragma once
#include "sema.hpp"
#include <optional>
#include <string>
#include <memory>
using namespace zap::sema;

void SymbolTable::addFunction(const FunctionSymbol &func)
{
    functions_.emplace(func.name, func);
}
std::shared_ptr<FunctionSymbol> SymbolTable::getFunction(const std::string &name)
{
    auto it = functions_.find(name);
    if (it != functions_.end())
    {
        return std::make_shared<FunctionSymbol>(it->second);
    }
    return nullptr;
}