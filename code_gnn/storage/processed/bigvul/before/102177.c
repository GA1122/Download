void SyncManager::RequestConfig(const syncable::ModelTypeBitSet& types,
    ConfigureReason reason) {
  if (!data_->scheduler()) {
    LOG(INFO)
        << "SyncManager::RequestConfig: bailing out because scheduler is "
        << "null";
    return;
  }
  StartConfigurationMode(NULL);
  data_->scheduler()->ScheduleConfig(types, GetSourceFromReason(reason));
}
