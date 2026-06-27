#include "ir/failable_type.hpp"
#include <cctype>

namespace zir {
namespace {

std::string sanitizeFailableTypeName(const std::string &value) {
  std::string out;
  out.reserve(value.size());
  for (char ch : value) {
    if (std::isalnum(static_cast<unsigned char>(ch))) {
      out.push_back(ch);
    } else {
      out.push_back('_');
    }
  }
  return out;
}

std::shared_ptr<RecordType>
asFailableRecord(const std::shared_ptr<Type> &type) {
  if (!type || type->getKind() != TypeKind::Record) {
    return nullptr;
  }

  auto record = std::static_pointer_cast<RecordType>(type);
  if (record->getName().rfind(kFailableTypePrefix, 0) != 0) {
    return nullptr;
  }

  const auto &fields = record->getFields();
  if (fields.size() != 3 || fields[FailableTypeLayout::OkField].name != "ok" ||
      fields[FailableTypeLayout::ValueField].name != "value" ||
      fields[FailableTypeLayout::ErrorField].name != "error") {
    return nullptr;
  }

  return record;
}

} // namespace

std::optional<FailableTypeLayout>
getFailableTypeLayout(const std::shared_ptr<Type> &type) {
  auto record = asFailableRecord(type);
  if (!record) {
    return std::nullopt;
  }
  return FailableTypeLayout{
      record->getFields()[FailableTypeLayout::ValueField].type,
      record->getFields()[FailableTypeLayout::ErrorField].type};
}

std::shared_ptr<RecordType>
makeFailableRecordType(const std::shared_ptr<Type> &valueType,
                       const std::shared_ptr<Type> &errorType) {
  auto suffix = sanitizeFailableTypeName(
      (valueType ? valueType->toString() : "<?>") + std::string("$") +
      (errorType ? errorType->toString() : "<?>"));
  auto typeName = std::string(kFailableTypePrefix) + suffix;
  auto type = std::make_shared<RecordType>(typeName, typeName);
  type->addField("ok", std::make_shared<PrimitiveType>(TypeKind::Bool));
  type->addField("value", valueType);
  type->addField("error", errorType);
  return type;
}

} // namespace zir
