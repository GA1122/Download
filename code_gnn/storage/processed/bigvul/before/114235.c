void CommandBufferProxyImpl::SetChannelErrorCallback(
    const base::Closure& callback) {
  channel_error_callback_ = callback;
}
