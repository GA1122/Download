void DevToolsAgentHostImpl::NotifyNavigated() {
  for (auto& observer : g_devtools_observers.Get())
    observer.DevToolsAgentHostNavigated(this);
}
