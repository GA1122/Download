void ProfileSyncService::StartSyncingWithServer() {
  if (backend_.get())
    backend_->StartSyncingWithServer();
}
