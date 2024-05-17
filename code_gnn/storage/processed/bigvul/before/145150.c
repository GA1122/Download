void GpuProcessHost::EstablishGpuChannel(
    int client_id,
    uint64_t client_tracing_id,
    bool preempts,
    bool allow_view_command_buffers,
    bool allow_real_time_streams,
    const EstablishChannelCallback& callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  TRACE_EVENT0("gpu", "GpuProcessHost::EstablishGpuChannel");

  if (!GpuDataManagerImpl::GetInstance()->GpuAccessAllowed(nullptr)) {
    DVLOG(1) << "GPU blacklisted, refusing to open a GPU channel.";
    callback.Run(mojo::ScopedMessagePipeHandle(), gpu::GPUInfo(),
                 gpu::GpuFeatureInfo(),
                 EstablishChannelStatus::GPU_ACCESS_DENIED);
    return;
  }

  if (gpu::IsReservedClientId(client_id)) {
    callback.Run(mojo::ScopedMessagePipeHandle(), gpu::GPUInfo(),
                 gpu::GpuFeatureInfo(),
                 EstablishChannelStatus::GPU_ACCESS_DENIED);
    return;
  }

  DCHECK_EQ(preempts, allow_view_command_buffers);
  DCHECK_EQ(preempts, allow_real_time_streams);
  bool is_gpu_host = preempts;
  bool cache_shaders_on_disk =
      GetShaderCacheFactorySingleton()->Get(client_id) != nullptr;

  channel_requests_.push(callback);
  gpu_service_ptr_->EstablishGpuChannel(
      client_id, client_tracing_id, is_gpu_host, cache_shaders_on_disk,
      base::BindOnce(&GpuProcessHost::OnChannelEstablished,
                     weak_ptr_factory_.GetWeakPtr(), client_id, callback));

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableGpuShaderDiskCache)) {
    CreateChannelCache(client_id);

    bool oopd_enabled = features::IsVizDisplayCompositorEnabled();
    if (oopd_enabled)
      CreateChannelCache(gpu::kInProcessCommandBufferClientId);

    bool oopr_enabled =
        base::FeatureList::IsEnabled(features::kDefaultEnableOopRasterization);
    if (oopr_enabled)
      CreateChannelCache(gpu::kGrShaderCacheClientId);
  }
}
