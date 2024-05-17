void CommandBufferProxyImpl::SetSwapBuffersCompletionCallback(
    const SwapBuffersCompletionCallback& callback) {
  CheckLock();
  swap_buffers_completion_callback_ = callback;
}
