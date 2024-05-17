void GpuCommandBufferStub::OnSignalSyncPoint(uint32 sync_point, uint32 id) {
  GpuChannelManager* manager = channel_->gpu_channel_manager();
  manager->sync_point_manager()->AddSyncPointCallback(
      sync_point,
      base::Bind(&GpuCommandBufferStub::OnSignalSyncPointAck,
                 this->AsWeakPtr(),
                 id));
}
