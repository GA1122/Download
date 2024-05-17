    WebContentsImpl::GetRootBrowserAccessibilityManager() {
  RenderFrameHostImpl* rfh = GetMainFrame();
  return rfh ? rfh->browser_accessibility_manager() : nullptr;
}
