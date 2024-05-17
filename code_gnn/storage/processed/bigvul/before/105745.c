bool XkbLayoutIsSupported(const std::string& xkb_layout) {
  static std::set<std::string>* g_supported_layouts = NULL;
  if (!g_supported_layouts) {
    g_supported_layouts = new std::set<std::string>;
    for (size_t i = 0; i < arraysize(kXkbLayoutsWhitelist); ++i) {
      g_supported_layouts->insert(kXkbLayoutsWhitelist[i]);
    }
  }
  return (g_supported_layouts->count(xkb_layout) > 0);
}
