bool GpuCommandBufferStub::IsScheduled() {
  return sync_point_wait_count_ == 0 &&
      (!scheduler_.get() || scheduler_->IsScheduled());
}
