void GpuCommandBufferStub::OnSyncPointRetired() {
  --sync_point_wait_count_;
  if (sync_point_wait_count_ == 0) {
    TRACE_EVENT_ASYNC_END1("gpu", "WaitSyncPoint", this,
                           "GpuCommandBufferStub", this);
  }
  OnReschedule();
}
