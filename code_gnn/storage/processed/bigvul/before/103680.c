void BrowserRenderProcessHost::OnUserMetricsRecordAction(
    const std::string& action) {
  UserMetrics::RecordComputedAction(action);
}
