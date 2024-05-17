bool GpuCommandBufferStub::IsScheduled() {
  return !scheduler_.get() || scheduler_->IsScheduled();
}
