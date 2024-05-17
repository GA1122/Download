void DevToolsAgentHostImpl::NotifyDestroyed() {
  DCHECK(g_devtools_instances.Get().find(id_) !=
         g_devtools_instances.Get().end());
  for (auto& observer : g_devtools_observers.Get())
    observer.DevToolsAgentHostDestroyed(this);
  g_devtools_instances.Get().erase(id_);
}
