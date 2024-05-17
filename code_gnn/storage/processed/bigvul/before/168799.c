void WebContentsImpl::RequestAXTreeSnapshot(AXTreeSnapshotCallback callback,
                                            ui::AXMode ax_mode) {
  FrameTreeNode* root_node = frame_tree_.root();
  AXTreeSnapshotCombiner* combiner =
      new AXTreeSnapshotCombiner(std::move(callback));

  RecursiveRequestAXTreeSnapshotOnFrame(root_node, combiner, ax_mode);
}
