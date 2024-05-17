void SyncBackendHost::Core::HandleInitalizationCompletedOnFrontendLoop() {
  if (!host_)
    return;
  host_->HandleInitializationCompletedOnFrontendLoop();
}
