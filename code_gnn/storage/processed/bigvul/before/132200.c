void RenderFrameImpl::FocusedNodeChangedForAccessibility(const WebNode& node) {
  if (renderer_accessibility())
    renderer_accessibility()->AccessibilityFocusedNodeChanged(node);
}
