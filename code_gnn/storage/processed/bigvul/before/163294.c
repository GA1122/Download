static void SetUpGLibLogHandler() {
  const char* const kLogDomains[] =
      { nullptr, "Gtk", "Gdk", "GLib", "GLib-GObject" };
  for (size_t i = 0; i < arraysize(kLogDomains); i++) {
    g_log_set_handler(
        kLogDomains[i],
        static_cast<GLogLevelFlags>(G_LOG_FLAG_RECURSION | G_LOG_FLAG_FATAL |
                                    G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL |
                                    G_LOG_LEVEL_WARNING),
        GLibLogHandler, nullptr);
  }
}
