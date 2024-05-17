void CommandBufferProxyImpl::FlushPendingWork() {
  if (!disconnected_)
    channel_->EnsureFlush(UINT32_MAX);
}
