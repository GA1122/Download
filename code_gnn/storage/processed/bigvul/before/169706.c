  std::string GetString(base::ListValue* value, uint32_t index) {
    std::string temp;
    if (!value->GetString(static_cast<size_t>(index), &temp)) {
      ADD_FAILURE();
      return std::string();
    }
    return temp;
  }
