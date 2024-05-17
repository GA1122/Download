void SyncBackendHost::Core::HandleActionableErrorEventOnFrontendLoop(
    const browser_sync::SyncProtocolError& sync_error) {
  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);
  if (!host_ || !host_->frontend_)
    return;
  host_->frontend_->OnActionableError(sync_error);
}
