void RenderWidgetHostImpl::SetAccessibilityMode(AccessibilityMode mode) {
  Send(new ViewMsg_SetAccessibilityMode(routing_id_, mode));
}
