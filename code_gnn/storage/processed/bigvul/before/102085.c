void SyncBackendHost::Core::OnActionableError(
    const browser_sync::SyncProtocolError& sync_error) {
  if (!host_ || !host_->frontend_)
    return;
  host_->frontend_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(this, &Core::HandleActionableErrorEventOnFrontendLoop,
                        sync_error));
}
