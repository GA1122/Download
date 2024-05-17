void GpuCommandBufferStub::PollWork() {
  if (decoder_.get()) {
    if (!decoder_->MakeCurrent()) {
      DLOG(ERROR) << "Context lost because MakeCurrent failed.";
      command_buffer_->SetContextLostReason(decoder_->GetContextLostReason());
      command_buffer_->SetParseError(gpu::error::kLostContext);
      if (gfx::GLContext::LosesAllContextsOnContextLost())
        channel_->LoseAllContexts();
      return;
    }
  }
  if (scheduler_.get())
    scheduler_->PollUnscheduleFences();
}
