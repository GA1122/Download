void SyncBackendHost::HandleClearServerDataSucceededOnFrontendLoop() {
  if (!frontend_)
    return;
  frontend_->OnClearServerDataSucceeded();
}
