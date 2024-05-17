void RenderWidgetHostImpl::OnMsgRenderViewReady() {
  SendScreenRects();
  WasResized();
}
