bool GpuCommandBufferStub::HasUnprocessedCommands() {
  if (command_buffer_.get()) {
    gpu::CommandBuffer::State state = command_buffer_->GetLastState();
    return state.put_offset != state.get_offset &&
        !gpu::error::IsError(state.error);
  }
  return false;
}
