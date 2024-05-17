void RenderFrameImpl::RequestOverlayRoutingToken(
    media::RoutingTokenCallback callback) {
  if (overlay_routing_token_.has_value()) {
    callback.Run(overlay_routing_token_.value());
    return;
  }

  Send(new FrameHostMsg_RequestOverlayRoutingToken(routing_id_));

  pending_routing_token_callbacks_.push_back(callback);
}
