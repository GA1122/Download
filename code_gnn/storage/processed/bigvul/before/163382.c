void RenderThreadImpl::PurgePluginListCache(bool reload_pages) {
#if BUILDFLAG(ENABLE_PLUGINS)
  blink_platform_impl_->set_plugin_refresh_allowed(false);
  blink::ResetPluginCache(reload_pages);
  blink_platform_impl_->set_plugin_refresh_allowed(true);

  for (auto& observer : observers_)
    observer.PluginListChanged();
#else
  NOTREACHED();
#endif
}
