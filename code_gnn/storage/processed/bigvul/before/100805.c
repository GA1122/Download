bool HttpResponseHeaders::HasHeaderValue(const std::string& name,
                                         const std::string& value) const {
  void* iter = NULL;
  std::string temp;
  while (EnumerateHeader(&iter, name, &temp)) {
    if (value.size() == temp.size() &&
        std::equal(temp.begin(), temp.end(), value.begin(),
                   base::CaseInsensitiveCompare<char>()))
      return true;
  }
  return false;
}
