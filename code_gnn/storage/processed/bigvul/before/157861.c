void WebContentsImpl::SetMainFrameImportance(
    ChildProcessImportance importance) {
  GetMainFrame()->GetRenderWidgetHost()->SetImportance(importance);
  if (ShowingInterstitialPage()) {
    static_cast<RenderFrameHostImpl*>(interstitial_page_->GetMainFrame())
        ->GetRenderWidgetHost()
        ->SetImportance(importance);
  }
}
