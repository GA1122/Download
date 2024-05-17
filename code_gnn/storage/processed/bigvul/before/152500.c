blink::WebFrame* RenderFrameImpl::ResolveOpener(int opener_frame_routing_id) {
  if (opener_frame_routing_id == MSG_ROUTING_NONE)
    return nullptr;

  RenderFrameProxy* opener_proxy =
      RenderFrameProxy::FromRoutingID(opener_frame_routing_id);
  if (opener_proxy)
    return opener_proxy->web_frame();

  RenderFrameImpl* opener_frame =
      RenderFrameImpl::FromRoutingID(opener_frame_routing_id);
  if (opener_frame)
    return opener_frame->GetWebFrame();

  return nullptr;
}
