void SyncBackendHost::Core::OnStopSyncingPermanently() {
  host_->frontend_loop_->PostTask(FROM_HERE, NewRunnableMethod(this,
      &Core::HandleStopSyncingPermanentlyOnFrontendLoop));
}
