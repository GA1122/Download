void WebContentsImpl::NotifyMainFrameSwappedFromRenderManager(
    RenderViewHost* old_host,
    RenderViewHost* new_host) {
  NotifyViewSwapped(old_host, new_host);
}
