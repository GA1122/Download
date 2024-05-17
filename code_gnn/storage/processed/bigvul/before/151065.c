DevToolsWindow* DevToolsWindow::FindDevToolsWindow(
    DevToolsAgentHost* agent_host) {
  if (!agent_host || g_instances == NULL)
    return NULL;
  DevToolsWindows* instances = g_instances.Pointer();
  for (DevToolsWindows::iterator it(instances->begin()); it != instances->end();
       ++it) {
    if ((*it)->bindings_->IsAttachedTo(agent_host))
      return *it;
  }
  return NULL;
}
