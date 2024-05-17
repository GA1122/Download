scoped_refptr<gpu::GpuChannelHost> RenderThreadImpl::EstablishGpuChannelSync(
    bool* connection_error) {
  TRACE_EVENT0("gpu", "RenderThreadImpl::EstablishGpuChannelSync");

  if (gpu_channel_) {
    if (!gpu_channel_->IsLost())
      return gpu_channel_;

    gpu_channel_->DestroyChannel();
    gpu_channel_ = nullptr;
  }

  gpu_channel_ = gpu_->EstablishGpuChannelSync(connection_error);
  if (gpu_channel_)
    GetContentClient()->SetGpuInfo(gpu_channel_->gpu_info());
  return gpu_channel_;
}
