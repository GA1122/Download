void RenderThreadImpl::AddRoute(int32_t routing_id, IPC::Listener* listener) {
  ChildThreadImpl::GetRouter()->AddRoute(routing_id, listener);
  auto it = pending_frame_creates_.find(routing_id);
  if (it == pending_frame_creates_.end())
    return;

  RenderFrameImpl* frame = RenderFrameImpl::FromRoutingID(routing_id);
  if (!frame)
    return;

  scoped_refptr<PendingFrameCreate> create(it->second);
  frame->BindFrame(it->second->browser_info(), it->second->TakeFrameRequest());
  pending_frame_creates_.erase(it);
}
