//
// Created by Funcieq on 27.11.2025.
//

#ifndef IGNIS_NODE_H
#define IGNIS_NODE_H

enum NodeType {
    TFun,
    TLet,
    TRet,
    TParam,
};

#include <string>
struct Node {
    NodeType nodeType;
    std::string_view funcName;
    std::vector<Node*> children;
};
#endif //IGNIS_NODE_H