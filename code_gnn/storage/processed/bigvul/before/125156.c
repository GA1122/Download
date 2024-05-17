PluginServiceImpl::~PluginServiceImpl() {
#if defined(OS_WIN)
  hkcu_watcher_.StopWatching();
  hklm_watcher_.StopWatching();
  if (hkcu_event_.get())
    hkcu_event_->Release();
  if (hklm_event_.get())
    hklm_event_->Release();
#endif
  DCHECK(pending_plugin_clients_.empty());
}
