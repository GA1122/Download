void SyncBackendHost::Core::DoRequestClearServerData() {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->RequestClearServerData();
}
