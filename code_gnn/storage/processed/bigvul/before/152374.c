void RenderFrameImpl::HandleAccessibilityFindInPageResult(
    int identifier,
    int match_index,
    const blink::WebNode& start_node,
    int start_offset,
    const blink::WebNode& end_node,
    int end_offset) {
  if (render_accessibility_) {
    render_accessibility_->HandleAccessibilityFindInPageResult(
        identifier, match_index, blink::WebAXObject::FromWebNode(start_node),
        start_offset, blink::WebAXObject::FromWebNode(end_node), end_offset);
  }
}
