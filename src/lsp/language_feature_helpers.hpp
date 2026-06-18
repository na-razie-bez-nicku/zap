#pragma once

#include "ast/nodes.hpp"
#include "ir/type.hpp"
#include "lsp.hpp"
#include "workspace_types.hpp"
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace zap::lsp {

std::optional<LspSymbol> makeSymbol(const std::string &uri,
                                    const std::string &name,
                                    const SourceSpan &span, int64_t kind,
                                    Visibility visibility);

std::optional<LspSignature> signatureForNode(const Node *node);

std::string effectiveImportAlias(const sema::ResolvedImport &import,
                                 const sema::ModuleInfo &target);

const sema::ModuleInfo *
findImportedModuleByAlias(const ProjectState &project,
                          const sema::ModuleInfo &module,
                          std::string_view alias);

std::pair<std::string, std::string>
splitQualifiedName(std::string_view qualifiedName);

std::string stripGenericArguments(std::string_view typeName);

const ClassDecl *resolveClassByTypeName(const ProjectState &project,
                                        const sema::ModuleInfo &module,
                                        std::string_view typeName,
                                        bool publicOnly);

std::optional<std::string>
resolveVariableClassType(const ProjectState &project,
                         const sema::ModuleInfo &module, size_t offset,
                         std::string_view name);

std::shared_ptr<zir::RecordType>
resolveVariableRecordType(const ProjectState &project,
                          const sema::ModuleInfo &module, size_t offset,
                          std::string_view name);

std::vector<LspSymbol> collectTopLevelSymbols(const sema::ModuleInfo &module,
                                              const std::string &uri,
                                              bool publicOnly);

std::vector<LspSymbol>
collectExportedSymbolsRecursive(const ProjectState &project,
                                const sema::ModuleInfo &module,
                                std::set<std::string> &visited);

std::vector<LspSymbol> collectImportedSymbols(const ProjectState &project,
                                              const sema::ModuleInfo &module);

} // namespace zap::lsp
