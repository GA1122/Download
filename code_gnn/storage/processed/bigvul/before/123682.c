void GpuCommandBufferStub::OnWaitSyncPoint(uint32 sync_point) {
  if (sync_point_wait_count_ == 0) {
    TRACE_EVENT_ASYNC_BEGIN1("gpu", "WaitSyncPoint", this,
                             "GpuCommandBufferStub", this);
  }
  ++sync_point_wait_count_;
  GpuChannelManager* manager = channel_->gpu_channel_manager();
  manager->sync_point_manager()->AddSyncPointCallback(
      sync_point,
      base::Bind(&GpuCommandBufferStub::OnSyncPointRetired,
                 this->AsWeakPtr()));
}
