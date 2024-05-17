void WebContentsImpl::NotifyMainFrameSwappedFromRenderManager(
    RenderFrameHost* old_host,
    RenderFrameHost* new_host) {
  NotifyViewSwapped(old_host ? old_host->GetRenderViewHost() : nullptr,
                    new_host->GetRenderViewHost());
}
