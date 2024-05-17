void RenderWidgetHostImpl::OnMsgRenderViewGone(int status, int exit_code) {
  Destroy();
}
