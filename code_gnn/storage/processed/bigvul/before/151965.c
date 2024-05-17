int RenderFrameHost::GetFrameTreeNodeIdForRoutingId(int process_id,
                                                    int routing_id) {
  RenderFrameHostImpl* rfh = nullptr;
  RenderFrameProxyHost* rfph = nullptr;
  LookupRenderFrameHostOrProxy(process_id, routing_id, &rfh, &rfph);
  if (rfh) {
    return rfh->GetFrameTreeNodeId();
  } else if (rfph) {
    return rfph->frame_tree_node()->frame_tree_node_id();
  }
  return kNoFrameTreeNodeId;
}
