void SyncBackendHost::Core::OnClearServerDataSucceeded() {
  host_->frontend_loop_->PostTask(FROM_HERE, NewRunnableMethod(this,
      &Core::HandleClearServerDataSucceededOnFrontendLoop));
}
