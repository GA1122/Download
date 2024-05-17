void WebContentsImpl::NotifyFrameSwapped(RenderFrameHost* old_host,
                                         RenderFrameHost* new_host) {
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    RenderFrameHostChanged(old_host, new_host));
}
