void RenderViewImpl::OnClose() {
  if (closing_)
    RenderThread::Get()->Send(new ViewHostMsg_Close_ACK(GetRoutingID()));
  RenderWidget::OnClose();
}
