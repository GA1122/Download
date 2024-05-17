 bool ContextualSearchFieldTrial::GetBooleanParam(const std::string& name,
                                                 bool* is_value_cached,
                                                 bool* cached_value) {
  if (!*is_value_cached) {
    *is_value_cached = true;
    std::string string_value = GetSwitch(name);
    bool has_switch = HasSwitch(name);
    if (has_switch && string_value.empty())
      string_value = kAnyNonEmptyValue;
    if (!has_switch)
      string_value = GetParam(name);
    *cached_value = !string_value.empty() && string_value != kFalseValue;
  }
  return *cached_value;
}
