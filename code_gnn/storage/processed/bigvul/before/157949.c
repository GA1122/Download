void RenderViewImpl::OnClosePage() {
  DCHECK(webview()->MainFrame()->IsWebLocalFrame());

  webview()->MainFrame()->ToWebLocalFrame()->DispatchUnloadEvent();

  Send(new ViewHostMsg_ClosePage_ACK(GetRoutingID()));
}
