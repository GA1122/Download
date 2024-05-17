void CommandBufferProxyImpl::OnDestroyed(gpu::error::ContextLostReason reason) {
  channel_ = NULL;

  last_state_.error = gpu::error::kLostContext;
  last_state_.context_lost_reason = reason;

  if (!channel_error_callback_.is_null()) {
    channel_error_callback_.Run();
    channel_error_callback_.Reset();
  }
}
