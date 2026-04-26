#pragma once
#include "attribute.hpp"
#include "node.hpp"
#include "visitor.hpp"
#include "../visibility.hpp"
#include <vector>

class TopLevel : public virtual Node {
public:
  Visibility visibility_ = Visibility::Private;
  std::vector<AttributeNode> attributes_;

  virtual ~TopLevel() noexcept = default;

  void accept(Visitor &v) override { v.visit(*this); }
};
