void WebContentsImpl::NotifyFrameSwapped(RenderFrameHost* old_host,
                                         RenderFrameHost* new_host) {
  for (auto& observer : observers_)
    observer.RenderFrameHostChanged(old_host, new_host);
}
