void RenderWidgetHostImpl::ExtendSelectionAndDelete(
    size_t before,
    size_t after) {
  Send(new ViewMsg_ExtendSelectionAndDelete(GetRoutingID(), before, after));
}
