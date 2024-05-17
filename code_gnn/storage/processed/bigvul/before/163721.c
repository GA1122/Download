void WebContentsImpl::SetImportance(ChildProcessImportance importance) {
  std::set<RenderWidgetHostImpl*> set;
  if (ShowingInterstitialPage()) {
    set.insert(
        static_cast<RenderFrameHostImpl*>(interstitial_page_->GetMainFrame())
            ->GetRenderWidgetHost());
  }
  for (RenderFrameHost* rfh : GetAllFrames())
    set.insert(static_cast<RenderFrameHostImpl*>(rfh)->GetRenderWidgetHost());
  for (RenderWidgetHostImpl* host : set) {
    DCHECK(host);
    host->SetImportance(importance);
  }
}
