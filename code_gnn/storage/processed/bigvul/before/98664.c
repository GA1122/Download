void RenderWidget::didFocus() {
  Send(new ViewHostMsg_FocusedNodeChanged(routing_id_));

  if (!has_focus_ && webwidget_) {
    MessageLoop::current()->PostTask(FROM_HERE,
        NewRunnableMethod(this, &RenderWidget::ClearFocus));
  }
}
