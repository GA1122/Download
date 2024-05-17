void RenderWidgetHostImpl::AccessibilitySetTextSelection(
    int object_id, int start_offset, int end_offset) {
  Send(new AccessibilityMsg_SetTextSelection(
      GetRoutingID(), object_id, start_offset, end_offset));
}
