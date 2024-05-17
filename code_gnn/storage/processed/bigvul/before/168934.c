void DevToolsAgentHostImpl::NotifyAttached() {
  for (auto& observer : g_devtools_observers.Get())
    observer.DevToolsAgentHostAttached(this);
}
