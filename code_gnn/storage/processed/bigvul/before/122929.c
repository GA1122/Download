void RenderWidgetHostImpl::AccessibilityDoDefaultAction(int object_id) {
  Send(new AccessibilityMsg_DoDefaultAction(GetRoutingID(), object_id));
}
