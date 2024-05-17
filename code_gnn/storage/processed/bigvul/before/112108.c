void SyncManager::RequestConfig(
    ModelTypeSet types, ConfigureReason reason) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!data_->scheduler()) {
    LOG(INFO)
        << "SyncManager::RequestConfig: bailing out because scheduler is "
        << "null";
    return;
  }
  StartConfigurationMode(base::Closure());
  data_->scheduler()->ScheduleConfig(types, GetSourceFromReason(reason));
}
