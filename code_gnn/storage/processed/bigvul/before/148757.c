void InterstitialPageImpl::Copy() {
  FrameTreeNode* focused_node = frame_tree_->GetFocusedFrame();
  if (!focused_node)
    return;

  focused_node->current_frame_host()->GetFrameInputHandler()->Copy();
  RecordAction(base::UserMetricsAction("Copy"));
}
