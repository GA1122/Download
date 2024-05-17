std::set<RenderWidgetHostImpl*> WebContentsImpl::GetAllRenderWidgetHosts() {
  std::set<RenderWidgetHostImpl*> set;
  if (ShowingInterstitialPage()) {
    auto* host =
        static_cast<RenderFrameHostImpl*>(interstitial_page_->GetMainFrame())
            ->GetRenderWidgetHost();
    DCHECK(host);
    set.insert(host);
  }
  for (RenderFrameHost* rfh : GetAllFrames()) {
    auto* host = static_cast<RenderFrameHostImpl*>(rfh)->GetRenderWidgetHost();
    DCHECK(host);
    set.insert(host);
  }
  return set;
}
