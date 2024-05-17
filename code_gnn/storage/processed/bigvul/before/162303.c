void CommandBufferProxyImpl::WaitSyncTokenHint(
    const gpu::SyncToken& sync_token) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError)
    return;

  pending_sync_token_fences_.push_back(sync_token);
}
