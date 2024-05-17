void SyncBackendHost::Core::HandleClearServerDataFailedOnFrontendLoop() {
  if (!host_ || !host_->frontend_)
    return;
  host_->frontend_->OnClearServerDataFailed();
}
