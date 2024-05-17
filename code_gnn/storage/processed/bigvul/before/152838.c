void MetricsWebContentsObserver::OnBrowserFeatureUsage(
    content::RenderFrameHost* render_frame_host,
    const mojom::PageLoadFeatures& new_features) {
  DCHECK_EQ(GetMainFrame(render_frame_host), web_contents()->GetMainFrame());

  if (!committed_load_) {
    RecordInternalError(ERR_BROWSER_USAGE_WITH_NO_RELEVANT_LOAD);
    return;
  }

  committed_load_->metrics_update_dispatcher()->UpdateFeatures(
      render_frame_host, new_features);
}
