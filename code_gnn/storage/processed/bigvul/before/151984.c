void RenderFrameHostImpl::InsertVisualStateCallback(
    VisualStateCallback callback) {
  static uint64_t next_id = 1;
  uint64_t key = next_id++;
  Send(new FrameMsg_VisualStateRequest(routing_id_, key));
  visual_state_callbacks_.emplace(key, std::move(callback));
}
