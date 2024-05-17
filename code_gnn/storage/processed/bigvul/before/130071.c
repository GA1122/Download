std::string ListValueToString(const base::ListValue& list_value) {
  std::string json;
  JSONStringValueSerializer serializer(&json);
  serializer.set_pretty_print(true);
  serializer.Serialize(list_value);
  return json;
}
