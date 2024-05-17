void CommandBufferProxyImpl::SetStateFromMessageReply(
    const gpu::CommandBuffer::State& state) {
  CheckLock();
  last_state_lock_.AssertAcquired();
  if (last_state_.error != gpu::error::kNoError)
    return;
  if (state.generation - last_state_.generation < 0x80000000U)
    last_state_ = state;
  if (last_state_.error != gpu::error::kNoError)
    OnGpuStateError();
}
