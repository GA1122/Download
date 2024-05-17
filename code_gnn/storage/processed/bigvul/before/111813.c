void SyncBackendHost::NotifyEncryptionComplete() {
  if (!frontend_)
    return;

  DCHECK_EQ(MessageLoop::current(), frontend_loop_);
  frontend_->OnEncryptionComplete();
}
