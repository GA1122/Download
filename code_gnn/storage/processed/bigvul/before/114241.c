void CommandBufferProxyImpl::SetOnConsoleMessageCallback(
    const GpuConsoleMessageCallback& callback) {
  console_message_callback_ = callback;
}
