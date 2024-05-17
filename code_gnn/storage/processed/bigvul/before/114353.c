void GpuChannel::AddRoute(int32 route_id, IPC::Channel::Listener* listener) {
  router_.AddRoute(route_id, listener);
}
