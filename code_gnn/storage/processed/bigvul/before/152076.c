ui::AXTreeID RenderFrameHostImpl::RoutingIDToAXTreeID(int routing_id) {
  RenderFrameHostImpl* rfh = nullptr;
  RenderFrameProxyHost* rfph = nullptr;
  LookupRenderFrameHostOrProxy(GetProcess()->GetID(), routing_id, &rfh, &rfph);
  if (rfph) {
    rfh = rfph->frame_tree_node()->current_frame_host();
  }

  if (!rfh)
    return ui::AXTreeIDUnknown();

  return rfh->GetAXTreeID();
}
