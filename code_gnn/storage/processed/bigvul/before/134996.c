void AppCacheUpdateJob::DeleteSoon() {
  ClearPendingMasterEntries();
  manifest_response_writer_.reset();
  storage_->CancelDelegateCallbacks(this);
  service_->RemoveObserver(this);
  service_ = NULL;

  if (group_) {
    group_->SetUpdateAppCacheStatus(AppCacheGroup::IDLE);
    group_ = NULL;
  }

  base::MessageLoop::current()->DeleteSoon(FROM_HERE, this);
}
