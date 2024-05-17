GpuChannelHost* RenderThread::EstablishGpuChannelSync(
    content::CauseForGpuLaunch cause_for_gpu_launch) {
  if (gpu_channel_.get()) {
    if (gpu_channel_->state() == GpuChannelHost::kUnconnected ||
        gpu_channel_->state() == GpuChannelHost::kConnected)
      return GetGpuChannel();

    if (gpu_channel_->state() == GpuChannelHost::kLost)
      gpu_channel_ = NULL;
  }

  if (!gpu_channel_.get())
    gpu_channel_ = new GpuChannelHost;

  IPC::ChannelHandle channel_handle;
  base::ProcessHandle renderer_process_for_gpu;
  GPUInfo gpu_info;
  if (!Send(new GpuHostMsg_EstablishGpuChannel(cause_for_gpu_launch,
                                               &channel_handle,
                                               &renderer_process_for_gpu,
                                               &gpu_info)) ||
      channel_handle.name.empty() ||
      renderer_process_for_gpu == base::kNullProcessHandle) {
    gpu_channel_ = NULL;
    return NULL;
  }

  gpu_channel_->set_gpu_info(gpu_info);
  content::GetContentClient()->SetGpuInfo(gpu_info);

  gpu_channel_->Connect(channel_handle, renderer_process_for_gpu);

  return GetGpuChannel();
}
