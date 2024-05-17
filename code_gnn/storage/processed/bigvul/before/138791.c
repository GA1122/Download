ui::AXTreeIDRegistry::AXTreeID RenderFrameHostImpl::RoutingIDToAXTreeID(
    int routing_id) {
  RenderFrameHostImpl* rfh = nullptr;
  RenderFrameProxyHost* rfph = RenderFrameProxyHost::FromID(
      GetProcess()->GetID(), routing_id);
  if (rfph) {
    FrameTree* frame_tree = rfph->frame_tree_node()->frame_tree();
    FrameTreeNode* frame_tree_node = frame_tree->FindByRoutingID(
        GetProcess()->GetID(), routing_id);
    rfh = frame_tree_node->render_manager()->current_frame_host();
  } else {
    rfh = RenderFrameHostImpl::FromID(GetProcess()->GetID(), routing_id);

    if (rfh &&
        rfh->frame_tree_node()->frame_tree() !=
            frame_tree_node()->frame_tree()) {
      AccessibilityFatalError();
      return ui::AXTreeIDRegistry::kNoAXTreeID;
    }
  }

  if (!rfh)
    return ui::AXTreeIDRegistry::kNoAXTreeID;

  return rfh->GetAXTreeID();
}
