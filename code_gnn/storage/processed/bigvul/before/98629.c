void RenderWidget::CallDoDeferredUpdate() {
  DoDeferredUpdate();

  if (pending_input_event_ack_.get())
    Send(pending_input_event_ack_.release());
}
