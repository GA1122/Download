std::string ToString(std::unique_ptr<base::DictionaryValue> value) {
  std::string json;
  base::JSONWriter::Write(*value, &json);
  return json;
}
