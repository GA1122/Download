void SyncBackendHost::Core::set_processing_passphrase() {
  DCHECK(MessageLoop::current() == host_->frontend_loop_);
  processing_passphrase_ = true;
}
