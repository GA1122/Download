void RenderFrameImpl::DidChangeFrameOwnerProperties(
    blink::WebFrame* child_frame,
    const blink::WebFrameOwnerProperties& frame_owner_properties) {
  Send(new FrameHostMsg_DidChangeFrameOwnerProperties(
      routing_id_, RenderFrame::GetRoutingIdForWebFrame(child_frame),
      ConvertWebFrameOwnerPropertiesToFrameOwnerProperties(
          frame_owner_properties)));
}
