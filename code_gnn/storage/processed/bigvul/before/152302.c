void RenderFrameImpl::EnterFullscreen(
    const blink::WebFullscreenOptions& options) {
  Send(new FrameHostMsg_EnterFullscreen(routing_id_, options));
}
