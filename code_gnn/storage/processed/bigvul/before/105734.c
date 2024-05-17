 bool InputMethodIdIsWhitelisted(const std::string& input_method_id) {
  static std::set<std::string>* g_supported_input_methods = NULL;
  if (!g_supported_input_methods) {
    g_supported_input_methods = new std::set<std::string>;
    for (size_t i = 0; i < arraysize(kInputMethodIdsWhitelist); ++i) {
      g_supported_input_methods->insert(kInputMethodIdsWhitelist[i]);
    }
  }
  return (g_supported_input_methods->count(input_method_id) > 0);
}
