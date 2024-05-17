void WebContentsImpl::NotifyViewSwapped(RenderViewHost* old_host,
                                        RenderViewHost* new_host) {
  notify_disconnection_ = true;
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    RenderViewHostChanged(old_host, new_host));

  RemoveBrowserPluginEmbedder();
}
