void RenderProcessHostImpl::OnUserMetricsRecordAction(
    const std::string& action) {
  RecordComputedAction(action);
}
