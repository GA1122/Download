void DevToolsAgentHostImpl::NotifyDetached() {
  for (auto& observer : g_devtools_observers.Get())
    observer.DevToolsAgentHostDetached(this);
}
