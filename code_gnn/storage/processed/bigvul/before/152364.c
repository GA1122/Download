int RenderFrame::GetRoutingIdForWebFrame(blink::WebFrame* web_frame) {
  if (!web_frame)
    return MSG_ROUTING_NONE;
  if (web_frame->IsWebRemoteFrame()) {
    return RenderFrameProxy::FromWebFrame(web_frame->ToWebRemoteFrame())
        ->routing_id();
  }
  return RenderFrameImpl::FromWebFrame(web_frame)->GetRoutingID();
}
