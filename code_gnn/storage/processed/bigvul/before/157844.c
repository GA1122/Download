void WebContentsImpl::RequestAXTreeSnapshot(AXTreeSnapshotCallback callback,
                                            ui::AXMode ax_mode) {
  FrameTreeNode* root_node = frame_tree_.root();
  auto combiner =
      base::MakeRefCounted<AXTreeSnapshotCombiner>(std::move(callback));

  RecursiveRequestAXTreeSnapshotOnFrame(root_node, combiner.get(), ax_mode);
}
