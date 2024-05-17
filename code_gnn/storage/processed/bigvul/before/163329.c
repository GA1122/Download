gpu::GpuChannelHost* RenderThreadImpl::GetGpuChannel() {
  if (!gpu_channel_)
    return nullptr;
  if (gpu_channel_->IsLost())
    return nullptr;
  return gpu_channel_.get();
}
