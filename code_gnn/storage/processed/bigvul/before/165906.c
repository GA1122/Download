std::unique_ptr<base::DictionaryValue> GetDevToolsInitiator(
    const WebString& initiator_str) {
  if (initiator_str.IsNull())
    return nullptr;
  std::unique_ptr<base::DictionaryValue> initiator =
      base::DictionaryValue::From(base::JSONReader::Read(initiator_str.Utf8()));
  if (!initiator)
    return nullptr;
  base::Value* parent = initiator->FindPath({"stack", "parent"});
  if (parent && parent->is_dict())
    parent->RemoveKey("parent");
  return initiator;
}
