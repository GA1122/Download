void GpuProcessHost::OnChannelEstablished(
    int client_id,
    const EstablishChannelCallback& callback,
    mojo::ScopedMessagePipeHandle channel_handle) {
  TRACE_EVENT0("gpu", "GpuProcessHost::OnChannelEstablished");
  DCHECK(!channel_requests_.empty());
  DCHECK(channel_requests_.front().Equals(callback));
  channel_requests_.pop();

  auto* gpu_data_manager = GpuDataManagerImpl::GetInstance();
  if (channel_handle.is_valid() &&
      !gpu_data_manager->GpuAccessAllowed(nullptr)) {
    gpu_service_ptr_->CloseChannel(client_id);
    callback.Run(mojo::ScopedMessagePipeHandle(), gpu::GPUInfo(),
                 gpu::GpuFeatureInfo(),
                 EstablishChannelStatus::GPU_ACCESS_DENIED);
    RecordLogMessage(logging::LOG_WARNING, "WARNING",
                     "Hardware acceleration is unavailable.");
    return;
  }

  callback.Run(std::move(channel_handle), gpu_data_manager->GetGPUInfo(),
               gpu_data_manager->GetGpuFeatureInfo(),
               EstablishChannelStatus::SUCCESS);
}
