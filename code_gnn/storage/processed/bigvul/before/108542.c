void QuotaManager::StartEviction() {
  DCHECK(!temporary_storage_evictor_.get());
  temporary_storage_evictor_.reset(new QuotaTemporaryStorageEvictor(
      this, kEvictionIntervalInMilliSeconds));
  if (desired_available_space_ >= 0)
    temporary_storage_evictor_->set_min_available_disk_space_to_start_eviction(
        desired_available_space_);
  temporary_storage_evictor_->Start();
}
