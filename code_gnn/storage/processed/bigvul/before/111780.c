void SyncBackendHost::Core::DoEnableEncryptEverything() {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->EnableEncryptEverything();
}
