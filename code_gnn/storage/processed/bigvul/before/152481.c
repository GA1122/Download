void RenderFrameImpl::PostAccessibilityEvent(const blink::WebAXObject& obj,
                                             ax::mojom::Event event) {
  if (render_accessibility_)
    render_accessibility_->HandleWebAccessibilityEvent(obj, event);
}
