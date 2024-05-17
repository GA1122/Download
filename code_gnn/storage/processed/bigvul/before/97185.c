void WebFrameLoaderClient::dispatchDidFailProvisionalLoad(
    const ResourceError& error) {
  if (error.domain() == kInternalErrorDomain &&
      error.errorCode() == ERR_POLICY_CHANGE) {
    webframe_->DidFail(cancelledError(error.failingURL()), true);
    return;
  }

  OwnPtr<WebPluginLoadObserver> plugin_load_observer = GetPluginLoadObserver();
  webframe_->DidFail(error, true);
  if (plugin_load_observer)
    plugin_load_observer->didFailLoading(error);
}
