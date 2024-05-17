const base::UnguessableToken& RenderFrameHostImpl::GetOverlayRoutingToken() {
  if (!overlay_routing_token_) {
    overlay_routing_token_ = base::UnguessableToken::Create();
    g_token_frame_map.Get().emplace(*overlay_routing_token_, this);
  }

  return *overlay_routing_token_;
}
