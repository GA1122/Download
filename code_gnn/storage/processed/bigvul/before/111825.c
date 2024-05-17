void SyncBackendHost::OnNigoriDownloadRetry() {
  DCHECK_EQ(MessageLoop::current(), frontend_loop_);
  if (!frontend_)
    return;

  frontend_->OnSyncConfigureRetry();
}
