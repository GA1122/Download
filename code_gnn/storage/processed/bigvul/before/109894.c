void GpuProcessHost::CreateCommandBufferError(
    const CreateCommandBufferCallback& callback, int32 route_id) {
  callback.Run(route_id);
}
