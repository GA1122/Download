    WebContentsImpl::GetOrCreateRootBrowserAccessibilityManager() {
  RenderFrameHostImpl* rfh = GetMainFrame();
  return rfh ? rfh->GetOrCreateBrowserAccessibilityManager() : nullptr;
}
