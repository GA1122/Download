bool XmlConfigParser::IsParsingData() const {
  const std::string data_path[] = {"response", "app", "data"};
  return elements_.size() == arraysize(data_path) &&
         std::equal(elements_.begin(), elements_.end(), data_path);
}
