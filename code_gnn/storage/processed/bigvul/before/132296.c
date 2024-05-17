void RenderFrameImpl::didChangeSandboxFlags(blink::WebFrame* child_frame,
                                            blink::WebSandboxFlags flags) {
  int frame_routing_id = MSG_ROUTING_NONE;
  if (child_frame->isWebRemoteFrame()) {
    frame_routing_id =
        RenderFrameProxy::FromWebFrame(child_frame)->routing_id();
  } else {
    frame_routing_id =
        RenderFrameImpl::FromWebFrame(child_frame)->GetRoutingID();
  }

  Send(new FrameHostMsg_DidChangeSandboxFlags(routing_id_, frame_routing_id,
                                              flags));
}
