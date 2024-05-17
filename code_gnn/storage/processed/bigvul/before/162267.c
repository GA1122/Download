void CommandBufferProxyImpl::LockAndDisconnectChannel() {
  base::Optional<base::AutoLock> hold;
  if (lock_)
    hold.emplace(*lock_);
  DisconnectChannel();
}
