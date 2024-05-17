void WebContentsImpl::RequestAXTreeSnapshot(
    const AXTreeSnapshotCallback& callback) {
  AXTreeSnapshotCombiner* combiner = new AXTreeSnapshotCombiner(callback);
  for (FrameTreeNode* frame_tree_node : frame_tree_.Nodes()) {
    bool is_root = frame_tree_node->parent() == nullptr;
    frame_tree_node->current_frame_host()->RequestAXTreeSnapshot(
        combiner->AddFrame(is_root));
  }
}
