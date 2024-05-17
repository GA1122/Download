void RenderWidget::DoDeferredClose() {
  Send(new ViewHostMsg_Close(routing_id_));
}
