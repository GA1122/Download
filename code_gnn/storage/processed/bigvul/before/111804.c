void SyncBackendHost::HandleStopSyncingPermanentlyOnFrontendLoop() {
  if (!frontend_)
    return;
  frontend_->OnStopSyncingPermanently();
}
