void RenderThread::OnPurgePluginListCache(bool reload_pages) {
  EnsureWebKitInitialized();
  plugin_refresh_allowed_ = false;
  WebKit::resetPluginCache(reload_pages);
  plugin_refresh_allowed_ = true;
}
