void GpuCommandBufferStub::OnRetireSyncPoint(uint32 sync_point) {
  DCHECK(!sync_points_.empty() && sync_points_.front() == sync_point);
  sync_points_.pop_front();
  GpuChannelManager* manager = channel_->gpu_channel_manager();
  manager->sync_point_manager()->RetireSyncPoint(sync_point);
}
