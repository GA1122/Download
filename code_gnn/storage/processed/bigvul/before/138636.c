void RenderFrameHostImpl::AdvanceFocus(blink::WebFocusType type,
                                       RenderFrameProxyHost* source_proxy) {
  DCHECK(!source_proxy ||
         (source_proxy->GetProcess()->GetID() == GetProcess()->GetID()));
  int32_t source_proxy_routing_id = MSG_ROUTING_NONE;
  if (source_proxy)
    source_proxy_routing_id = source_proxy->GetRoutingID();
  Send(
      new FrameMsg_AdvanceFocus(GetRoutingID(), type, source_proxy_routing_id));
}
