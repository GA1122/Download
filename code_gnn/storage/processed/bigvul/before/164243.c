void AppCacheUpdateJob::DeleteSoon() {
  ClearPendingMasterEntries();
  manifest_response_writer_.reset();
  storage_->CancelDelegateCallbacks(this);
  service_->RemoveObserver(this);
  service_ = nullptr;

  if (group_) {
    group_->SetUpdateAppCacheStatus(AppCacheGroup::IDLE);
    group_ = nullptr;
  }

  base::ThreadTaskRunnerHandle::Get()->DeleteSoon(FROM_HERE, this);
}
