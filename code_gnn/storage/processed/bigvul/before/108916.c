void RenderViewImpl::OnClosePage() {
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, ClosePage());
  webview()->dispatchUnloadEvent();

  Send(new ViewHostMsg_ClosePage_ACK(routing_id_));
}
