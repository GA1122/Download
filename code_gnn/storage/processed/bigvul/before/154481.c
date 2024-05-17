void GLES2DecoderPassthroughImpl::MarkContextLost(
    error::ContextLostReason reason) {
  if (WasContextLost()) {
    return;
  }

  command_buffer_service()->SetContextLostReason(reason);
  context_lost_ = true;
}
