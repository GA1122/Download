RenderWidgetHostImpl* WebContentsImpl::GetRenderWidgetHostWithPageFocus() {
  WebContentsImpl* focused_web_contents = GetFocusedWebContents();

  if (focused_web_contents->ShowingInterstitialPage()) {
    return static_cast<RenderFrameHostImpl*>(
               focused_web_contents->interstitial_page_->GetMainFrame())
        ->GetRenderWidgetHost();
  }
  if (!GuestMode::IsCrossProcessFrameGuest(focused_web_contents) &&
      focused_web_contents->browser_plugin_guest_) {
    return focused_web_contents->GetOuterWebContents()
        ->GetMainFrame()
        ->GetRenderWidgetHost();
  }

  return focused_web_contents->GetMainFrame()->GetRenderWidgetHost();
}
