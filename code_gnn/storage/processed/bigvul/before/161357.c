std::unique_ptr<Object> getHeaders(const base::StringPairs& pairs) {
  std::unique_ptr<DictionaryValue> headers_dict(DictionaryValue::create());
  for (const auto& pair : pairs) {
    headers_dict->setString(pair.first, pair.second);
  }
  return Object::fromValue(headers_dict.get(), nullptr);
}
