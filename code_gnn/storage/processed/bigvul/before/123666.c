void GpuCommandBufferStub::OnGetState(IPC::Message* reply_message) {
  TRACE_EVENT0("gpu", "GpuCommandBufferStub::OnGetState");
  if (command_buffer_.get()) {
    gpu::CommandBuffer::State state = command_buffer_->GetState();
    if (state.error == gpu::error::kLostContext &&
        gfx::GLContext::LosesAllContextsOnContextLost())
      channel_->LoseAllContexts();

    GpuCommandBufferMsg_GetState::WriteReplyParams(reply_message, state);
  } else {
    DLOG(ERROR) << "no command_buffer.";
    reply_message->set_reply_error();
  }
  Send(reply_message);
}
