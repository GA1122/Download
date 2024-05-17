void WebContentsImpl::OnUpdatePageImportanceSignals(
    RenderFrameHostImpl* source,
    const PageImportanceSignals& signals) {
  page_importance_signals_ = signals;
}
