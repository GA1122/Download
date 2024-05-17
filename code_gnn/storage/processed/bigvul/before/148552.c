void WebContentsImpl::SetAccessibilityMode(AccessibilityMode mode) {
  if (mode == accessibility_mode_)
    return;

  if (IsNeverVisible())
    return;

  accessibility_mode_ = mode;

  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    UpdateAccessibilityModeOnFrame(node->current_frame_host());
    RenderFrameHost* pending_frame_host =
        node->render_manager()->pending_frame_host();
    if (pending_frame_host)
      UpdateAccessibilityModeOnFrame(pending_frame_host);
  }
}
