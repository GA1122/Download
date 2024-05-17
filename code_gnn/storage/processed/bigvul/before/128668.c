int ContextualSearchFieldTrial::GetIntParamValueOrDefault(
    const std::string& name,
    const int default_value,
    bool* is_value_cached,
    int* cached_value) {
  if (!*is_value_cached) {
    *is_value_cached = true;
    std::string param_string = GetSwitch(name);
    if (param_string.empty())
      param_string = GetParam(name);

    int param_int;
    if (!param_string.empty() && base::StringToInt(param_string, &param_int))
      *cached_value = param_int;
    else
      *cached_value = default_value;
  }
  return *cached_value;
}
