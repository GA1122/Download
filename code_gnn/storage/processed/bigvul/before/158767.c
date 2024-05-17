void WebContentsImpl::FullscreenStateChanged(RenderFrameHost* rfh,
                                             bool is_fullscreen) {
  int frame_tree_node_id = rfh->GetFrameTreeNodeId();
  auto it = fullscreen_frame_tree_nodes_.find(frame_tree_node_id);
  bool changed = false;

  if (is_fullscreen) {
    if (it == fullscreen_frame_tree_nodes_.end()) {
      fullscreen_frame_tree_nodes_.insert(frame_tree_node_id);
      changed = true;
    }
  } else {
    FrameTreeNode* ancestor =
        static_cast<RenderFrameHostImpl*>(rfh)->frame_tree_node();
    DCHECK(ancestor);

    for (it = fullscreen_frame_tree_nodes_.begin();
         it != fullscreen_frame_tree_nodes_.end();) {
      FrameTreeNode* node = FrameTreeNode::GloballyFindByID(*it);

      if (!node || frame_tree_node_id == *it ||
          node->IsDescendantOf(ancestor)) {
        it = fullscreen_frame_tree_nodes_.erase(it);
        changed = true;
      } else {
        ++it;
      }
    }
  }

  if (changed && fullscreen_frame_tree_nodes_.size() > 0) {
    unsigned int max_depth = 0;
    RenderFrameHost* max_depth_rfh = nullptr;

    for (auto node_id : fullscreen_frame_tree_nodes_) {
      FrameTreeNode* fullscreen_node = FrameTreeNode::GloballyFindByID(node_id);
      DCHECK(fullscreen_node);

      if (max_depth_rfh == nullptr || fullscreen_node->depth() > max_depth) {
        max_depth = fullscreen_node->depth();
        max_depth_rfh = fullscreen_node->current_frame_host();
      }
    }

    DCHECK(max_depth_rfh);
    if (max_depth_rfh->GetFrameTreeNodeId() ==
        current_fullscreen_frame_tree_node_id_)
      return;

    current_fullscreen_frame_tree_node_id_ =
        max_depth_rfh->GetFrameTreeNodeId();

    for (auto& observer : observers_)
      observer.DidAcquireFullscreen(max_depth_rfh);

    if (display_cutout_host_impl_)
      display_cutout_host_impl_->DidAcquireFullscreen(max_depth_rfh);
  } else if (fullscreen_frame_tree_nodes_.size() == 0) {
    current_fullscreen_frame_tree_node_id_ =
        RenderFrameHost::kNoFrameTreeNodeId;
  }
}
