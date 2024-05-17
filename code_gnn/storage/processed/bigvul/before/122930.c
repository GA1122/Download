void RenderWidgetHostImpl::AccessibilityScrollToMakeVisible(
    int acc_obj_id, gfx::Rect subfocus) {
  Send(new AccessibilityMsg_ScrollToMakeVisible(
      GetRoutingID(), acc_obj_id, subfocus));
}
