void SyncBackendHost::Core::OnBootstrapTokenUpdated(
    const std::string& bootstrap_token) {
  if (!sync_loop_)
    return;
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  host_.Call(
      FROM_HERE,
      &SyncBackendHost::PersistEncryptionBootstrapToken, bootstrap_token);
}
