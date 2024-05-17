void PluginServiceImpl::OnWaitableEventSignaled(
    base::WaitableEvent* waitable_event) {
#if defined(OS_WIN)
  if (waitable_event == hkcu_event_.get()) {
    hkcu_key_.StartWatching();
  } else {
    hklm_key_.StartWatching();
  }

  plugin_list_->RefreshPlugins();
  PurgePluginListCache(NULL, false);
#else
  NOTREACHED();
#endif   
}
