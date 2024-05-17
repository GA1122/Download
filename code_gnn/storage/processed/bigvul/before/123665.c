void GpuCommandBufferStub::OnEnsureBackbuffer() {
  TRACE_EVENT0("gpu", "GpuCommandBufferStub::OnEnsureBackbuffer");
  if (!surface_)
    return;
  if (surface_->DeferDraws()) {
    DCHECK(!IsScheduled());
    channel_->RequeueMessage();
  } else {
    surface_->SetBackbufferAllocation(true);
  }
}
