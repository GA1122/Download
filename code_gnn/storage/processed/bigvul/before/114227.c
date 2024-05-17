void CommandBufferProxyImpl::OnConsoleMessage(
    const GPUCommandBufferConsoleMessage& message) {
  if (!console_message_callback_.is_null()) {
    console_message_callback_.Run(message.message, message.id);
  }
}
