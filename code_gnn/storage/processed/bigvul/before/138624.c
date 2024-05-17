void RenderFrameHostImpl::AXContentTreeDataToAXTreeData(
    ui::AXTreeData* dst) {
  const AXContentTreeData& src = ax_content_tree_data_;

  *dst = src;

  if (src.routing_id != -1)
    dst->tree_id = RoutingIDToAXTreeID(src.routing_id);

  if (src.parent_routing_id != -1)
    dst->parent_tree_id = RoutingIDToAXTreeID(src.parent_routing_id);

  if (browser_plugin_embedder_ax_tree_id_ != ui::AXTreeIDRegistry::kNoAXTreeID)
    dst->parent_tree_id = browser_plugin_embedder_ax_tree_id_;

  if (frame_tree_node()->parent())
    return;

  FrameTreeNode* focused_frame_tree_node = frame_tree_->GetFocusedFrame();
  if (!focused_frame_tree_node)
    return;
  RenderFrameHostImpl* focused_frame =
      focused_frame_tree_node->current_frame_host();
  DCHECK(focused_frame);
  dst->focused_tree_id = focused_frame->GetAXTreeID();
}
