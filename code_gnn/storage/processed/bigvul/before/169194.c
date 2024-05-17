RenderFrameHost* ChildFrameAt(RenderFrameHost* frame, size_t index) {
  RenderFrameHostImpl* rfh = static_cast<RenderFrameHostImpl*>(frame);
  if (index >= rfh->frame_tree_node()->child_count())
    return nullptr;
  return rfh->frame_tree_node()->child_at(index)->current_frame_host();
}
