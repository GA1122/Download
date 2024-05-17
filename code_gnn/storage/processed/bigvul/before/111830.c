void SyncBackendHost::Core::OnUpdatedToken(const std::string& token) {
  if (!sync_loop_)
    return;
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  host_.Call(
      FROM_HERE,
      &SyncBackendHost::NotifyUpdatedToken, token);
}
