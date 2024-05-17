void WebFrameLoaderClient::dispatchDidFailLoad(const ResourceError& error) {
  OwnPtr<WebPluginLoadObserver> plugin_load_observer = GetPluginLoadObserver();
  webframe_->DidFail(error, false);
  if (plugin_load_observer)
    plugin_load_observer->didFailLoading(error);

}
