void WebFrameLoaderClient::didFinishLoad() {
  OwnPtr<WebPluginLoadObserver> plugin_load_observer = GetPluginLoadObserver();
  if (plugin_load_observer)
    plugin_load_observer->didFinishLoading();
}
