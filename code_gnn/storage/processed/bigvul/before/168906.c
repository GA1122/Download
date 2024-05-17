void DevToolsAgentHost::AddObserver(DevToolsAgentHostObserver* observer) {
  if (observer->ShouldForceDevToolsAgentHostCreation()) {
    if (!DevToolsAgentHostImpl::s_force_creation_count_) {
      DevToolsAgentHost::GetOrCreateAll();
    }
    DevToolsAgentHostImpl::s_force_creation_count_++;
  }

  g_devtools_observers.Get().AddObserver(observer);
  for (const auto& id_host : g_devtools_instances.Get())
    observer->DevToolsAgentHostCreated(id_host.second);
}
