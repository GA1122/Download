void GpuProcessHost::EstablishChannelError(
    const EstablishChannelCallback& callback,
    const IPC::ChannelHandle& channel_handle,
    base::ProcessHandle renderer_process_for_gpu,
    const GPUInfo& gpu_info) {
  callback.Run(channel_handle, gpu_info);
}
