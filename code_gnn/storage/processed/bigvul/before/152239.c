RenderFrameImpl::CreateWebSocketHandshakeThrottle() {
  WebLocalFrame* web_local_frame = GetWebFrame();
  if (!web_local_frame)
    return nullptr;
  auto* render_frame = content::RenderFrame::FromWebFrame(web_local_frame);
  if (!render_frame)
    return nullptr;
  int render_frame_id = render_frame->GetRoutingID();

  if (!websocket_handshake_throttle_provider_) {
    websocket_handshake_throttle_provider_ =
        GetContentClient()
            ->renderer()
            ->CreateWebSocketHandshakeThrottleProvider();
    if (!websocket_handshake_throttle_provider_)
      return nullptr;
  }

  return websocket_handshake_throttle_provider_->CreateThrottle(
      render_frame_id,
      render_frame->GetTaskRunner(blink::TaskType::kInternalDefault));
}
