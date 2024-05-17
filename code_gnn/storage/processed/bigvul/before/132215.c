void RenderFrameImpl::HandleWebAccessibilityEvent(
    const blink::WebAXObject& obj, blink::WebAXEvent event) {
  if (renderer_accessibility_)
    renderer_accessibility_->HandleWebAccessibilityEvent(obj, event);
}
