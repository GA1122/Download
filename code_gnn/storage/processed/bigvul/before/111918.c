void ProfileSyncService::OnSyncConfigureRetry() {
  if (!auto_start_enabled_ && !backend_initialized_) {
    OnUnrecoverableError(FROM_HERE,
                         "Configure failed to download.");
  }

  NotifyObservers();
}
