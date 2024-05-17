bool SyncBackendHost::Core::processing_passphrase() const {
  DCHECK(MessageLoop::current() == host_->frontend_loop_);
  return processing_passphrase_;
}
