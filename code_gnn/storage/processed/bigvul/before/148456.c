void WebContentsImpl::NotifyViewSwapped(RenderViewHost* old_host,
                                        RenderViewHost* new_host) {
  notify_disconnection_ = true;
  for (auto& observer : observers_)
    observer.RenderViewHostChanged(old_host, new_host);

  RemoveBrowserPluginEmbedder();
}
