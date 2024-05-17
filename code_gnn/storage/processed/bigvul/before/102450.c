ScopedPortException::~ScopedPortException() {
  std::multiset<int>::iterator it =
      g_explicitly_allowed_ports.Get().find(port_);
  if (it != g_explicitly_allowed_ports.Get().end())
    g_explicitly_allowed_ports.Get().erase(it);
  else
    NOTREACHED();
}
