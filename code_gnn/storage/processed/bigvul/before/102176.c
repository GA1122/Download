void SyncManager::RequestClearServerData() {
  if (data_->scheduler())
    data_->scheduler()->ScheduleClearUserData();
}
