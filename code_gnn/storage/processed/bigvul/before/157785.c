    WebContentsImpl::GetRootBrowserAccessibilityManager() {
  RenderFrameHostImpl* rfh = static_cast<RenderFrameHostImpl*>(
      ShowingInterstitialPage() ? GetInterstitialPage()->GetMainFrame()
                                : GetMainFrame());
  return rfh ? rfh->browser_accessibility_manager() : nullptr;
}
