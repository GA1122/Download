void SyncBackendHost::Core::DoRefreshNigori(
    const base::Closure& done_callback) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  chrome::VersionInfo version_info;
  sync_manager_->RefreshNigori(version_info.CreateVersionString(),
                               done_callback);
}
