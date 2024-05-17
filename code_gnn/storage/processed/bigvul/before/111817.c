void SyncBackendHost::Core::OnActionableError(
    const browser_sync::SyncProtocolError& sync_error) {
  if (!sync_loop_)
    return;
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  host_.Call(
      FROM_HERE,
      &SyncBackendHost::HandleActionableErrorEventOnFrontendLoop,
      sync_error);
}
