void RenderWidget::OnClose() {
  if (closing_)
    return;
  closing_ = true;

  if (routing_id_ != MSG_ROUTING_NONE) {
    render_thread_->RemoveRoute(routing_id_);
    SetHidden(false);
  }

  MessageLoop::current()->PostNonNestableTask(FROM_HERE,
      NewRunnableMethod(this, &RenderWidget::Close));

  Release();
}
