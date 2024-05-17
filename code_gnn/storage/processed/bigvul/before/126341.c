GQuark GetBrowserWindowQuarkKey() {
  static GQuark quark = g_quark_from_static_string(kBrowserWindowKey);
  return quark;
}
