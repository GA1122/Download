void WebContentsImpl::SetAccessibilityMode(ui::AXMode mode) {
  if (mode == accessibility_mode_)
    return;

  if (IsNeverVisible())
    return;

  accessibility_mode_ = mode;

  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    UpdateAccessibilityModeOnFrame(node->current_frame_host());
    RenderFrameHost* speculative_frame_host =
        node->render_manager()->speculative_frame_host();
    if (speculative_frame_host)
      UpdateAccessibilityModeOnFrame(speculative_frame_host);
  }
}
