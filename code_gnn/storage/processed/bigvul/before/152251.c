void RenderFrameImpl::DidChangeFramePolicy(
    blink::WebFrame* child_frame,
    const blink::FramePolicy& frame_policy) {
  Send(new FrameHostMsg_DidChangeFramePolicy(
      routing_id_, RenderFrame::GetRoutingIdForWebFrame(child_frame),
      frame_policy));
}
