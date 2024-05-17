bool IsPortAllowedByOverride(int port) {
  if (g_explicitly_allowed_ports.Get().empty())
    return false;

  return g_explicitly_allowed_ports.Get().count(port) > 0;
}
