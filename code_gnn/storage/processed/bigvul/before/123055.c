void RenderWidgetHostImpl::SetBackground(const SkBitmap& background) {
  Send(new ViewMsg_SetBackground(GetRoutingID(), background));
}
