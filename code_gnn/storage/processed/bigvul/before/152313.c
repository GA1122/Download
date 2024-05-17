void RenderFrameImpl::FocusedNodeChangedForAccessibility(const WebNode& node) {
  if (render_accessibility())
    render_accessibility()->AccessibilityFocusedNodeChanged(node);
}
