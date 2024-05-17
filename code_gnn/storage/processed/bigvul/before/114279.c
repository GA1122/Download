bool WebGraphicsContext3DCommandBufferImpl::IsCommandBufferContextLost() {
  if (host_ && host_->state() == GpuChannelHost::kLost)
    return true;
  gpu::CommandBuffer::State state = command_buffer_->GetLastState();
  return state.error == gpu::error::kLostContext;
}
