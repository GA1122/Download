void GpuProcessHost::OnCommandBufferCreated(const int32 route_id) {
  if (!create_command_buffer_requests_.empty()) {
    CreateCommandBufferCallback callback =
        create_command_buffer_requests_.front();
    create_command_buffer_requests_.pop();
    if (route_id == MSG_ROUTING_NONE)
      CreateCommandBufferError(callback, route_id);
    else
      callback.Run(route_id);
  }
}
