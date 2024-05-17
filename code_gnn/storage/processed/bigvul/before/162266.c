bool CommandBufferProxyImpl::IsFenceSyncReleased(uint64_t release) {
  base::AutoLock lock(last_state_lock_);
  TryUpdateStateThreadSafe();
  return release <= last_state_.release_count;
}
