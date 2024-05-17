CommandBufferProxyImpl::CommandBufferProxyImpl(
    scoped_refptr<GpuChannelHost> channel,
    GpuMemoryBufferManager* gpu_memory_buffer_manager,
    int32_t stream_id,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : channel_(std::move(channel)),
      gpu_memory_buffer_manager_(gpu_memory_buffer_manager),
      channel_id_(channel_->channel_id()),
      route_id_(channel_->GenerateRouteID()),
      stream_id_(stream_id),
      command_buffer_id_(CommandBufferProxyID(channel_id_, route_id_)),
      callback_thread_(std::move(task_runner)),
      weak_ptr_factory_(this) {
  DCHECK(route_id_);
}
