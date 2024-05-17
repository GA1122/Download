void ChromeMetricsServiceClient::OnSyncPrefsChanged(bool must_purge) {
  if (!ukm_service_)
    return;
  if (must_purge) {
    ukm_service_->Purge();
    ukm_service_->ResetClientState(ukm::ResetReason::kOnSyncPrefsChanged);
  }
  UpdateRunningServices();
}
