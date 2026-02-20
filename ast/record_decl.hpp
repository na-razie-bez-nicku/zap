// ast/record_decl.hpp
#pragma once
#include "node.hpp"
#include "top_level.hpp"
#include "parameter_node.hpp"
#include <string>
#include <vector>
#include <memory>

class RecordDecl : public TopLevel {
public:
    std::string name_;
    std::vector<std::unique_ptr<ParameterNode>> fields_;

    RecordDecl(const std::string& name, std::vector<std::unique_ptr<ParameterNode>> fields)
        : name_(name), fields_(std::move(fields)) {}
};
