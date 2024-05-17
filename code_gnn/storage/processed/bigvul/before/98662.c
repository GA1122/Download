void RenderWidget::didBlur() {
  Send(new ViewHostMsg_Blur(routing_id_));
}
