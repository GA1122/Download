RenderFrameImpl* RenderFrameImpl::FromRoutingID(int routing_id) {
  DCHECK(RenderThread::IsMainThread());
  auto iter = g_routing_id_frame_map.Get().find(routing_id);
  if (iter != g_routing_id_frame_map.Get().end())
    return iter->second;
  return nullptr;
}
