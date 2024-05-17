  std::string GetString(base::DictionaryValue* value, const std::string& key) {
    std::string temp;
    if (!value->GetString(key, &temp)) {
      ADD_FAILURE();
      return std::string();
    }
    return temp;
  }
