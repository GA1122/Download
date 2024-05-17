void SyncBackendHost::HandleInitializationCompletedOnFrontendLoop() {
  if (!frontend_)
    return;
  syncapi_initialized_ = true;
  frontend_->OnBackendInitialized();
}
