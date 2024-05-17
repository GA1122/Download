DictionaryValue* BaseNode::GetSummaryAsValue() const {
  DictionaryValue* node_info = new DictionaryValue();
  node_info->SetString("id", base::Int64ToString(GetId()));
  node_info->SetBoolean("isFolder", GetIsFolder());
  node_info->SetString("title", GetTitle());
  node_info->Set("type", ModelTypeToValue(GetModelType()));
  return node_info;
}
