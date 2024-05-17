void RenderFrameImpl::PostAccessibilityEvent(const blink::WebAXObject& obj,
                                             blink::WebAXEvent event) {
  HandleWebAccessibilityEvent(obj, event);
}
