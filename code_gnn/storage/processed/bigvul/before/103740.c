void RenderThread::AddRoute(int32 routing_id,
                            IPC::Channel::Listener* listener) {
  widget_count_++;
  return ChildThread::AddRoute(routing_id, listener);
}
