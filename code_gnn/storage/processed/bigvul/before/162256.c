void CommandBufferProxyImpl::EnsureWorkVisible() {
  if (!disconnected_)
    channel_->VerifyFlush(UINT32_MAX);
}
