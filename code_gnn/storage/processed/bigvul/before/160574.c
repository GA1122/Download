void RenderFrameImpl::DidChangeFramePolicy(
    blink::WebFrame* child_frame,
    blink::WebSandboxFlags flags,
    const blink::ParsedFeaturePolicy& container_policy) {
  Send(new FrameHostMsg_DidChangeFramePolicy(
      routing_id_, RenderFrame::GetRoutingIdForWebFrame(child_frame),
      {flags, container_policy}));
}
