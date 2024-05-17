void WebContentsImpl::OnUpdatePageImportanceSignals(
    const PageImportanceSignals& signals) {
  page_importance_signals_ = signals;
}
