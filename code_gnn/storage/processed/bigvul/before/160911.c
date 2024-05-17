void DOMWindow::Trace(blink::Visitor* visitor) {
  visitor->Trace(frame_);
  visitor->Trace(window_proxy_manager_);
  visitor->Trace(input_capabilities_);
  visitor->Trace(location_);
  EventTargetWithInlineData::Trace(visitor);
}
