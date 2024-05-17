void RenderWidgetHostImpl::AccessibilityScrollToPoint(
    int acc_obj_id, gfx::Point point) {
  Send(new AccessibilityMsg_ScrollToPoint(
      GetRoutingID(), acc_obj_id, point));
}
