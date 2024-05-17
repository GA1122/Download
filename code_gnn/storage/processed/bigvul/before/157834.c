void WebContentsImpl::RecursiveRequestAXTreeSnapshotOnFrame(
    FrameTreeNode* root_node,
    AXTreeSnapshotCombiner* combiner,
    ui::AXMode ax_mode) {
  for (FrameTreeNode* frame_tree_node : frame_tree_.Nodes()) {
    WebContentsImpl* inner_contents =
        node_.GetInnerWebContentsInFrame(frame_tree_node);
    if (inner_contents) {
      inner_contents->RecursiveRequestAXTreeSnapshotOnFrame(root_node, combiner,
                                                            ax_mode);
    } else {
      bool is_root = frame_tree_node == root_node;
      frame_tree_node->current_frame_host()->RequestAXTreeSnapshot(
          combiner->AddFrame(is_root), ax_mode);
    }
  }
}
