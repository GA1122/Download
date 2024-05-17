void GpuCommandBufferStub::OnDiscardBackbuffer() {
  TRACE_EVENT0("gpu", "GpuCommandBufferStub::OnDiscardBackbuffer");
  if (!surface_)
    return;
  if (surface_->DeferDraws()) {
    DCHECK(!IsScheduled());
    channel_->RequeueMessage();
  } else {
    surface_->SetBackbufferAllocation(false);
  }
}
