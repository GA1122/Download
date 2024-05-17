void InterstitialPageImpl::Paste() {
  FrameTreeNode* focused_node = frame_tree_->GetFocusedFrame();
  if (!focused_node)
    return;

  focused_node->current_frame_host()->GetFrameInputHandler()->Paste();
  RecordAction(base::UserMetricsAction("Paste"));
}
