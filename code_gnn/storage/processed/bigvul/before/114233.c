void CommandBufferProxyImpl::OnUpdateState(
    const gpu::CommandBuffer::State& state) {
  if (state.generation - last_state_.generation < 0x80000000U)
    last_state_ = state;
}
