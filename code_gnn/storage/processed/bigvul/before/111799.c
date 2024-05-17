void SyncBackendHost::HandleClearServerDataFailedOnFrontendLoop() {
  if (!frontend_)
    return;
  frontend_->OnClearServerDataFailed();
}
