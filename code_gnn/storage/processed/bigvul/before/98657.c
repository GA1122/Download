bool RenderWidget::Send(IPC::Message* message) {
  if (closing_) {
    delete message;
    return false;
  }

  if (message->routing_id() == MSG_ROUTING_NONE)
    message->set_routing_id(routing_id_);

  return render_thread_->Send(message);
}
