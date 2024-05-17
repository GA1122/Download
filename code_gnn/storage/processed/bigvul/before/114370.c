void GpuChannel::OnScheduled() {
  if (handle_messages_scheduled_)
    return;
  MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&GpuChannel::HandleMessage, weak_factory_.GetWeakPtr()));
  handle_messages_scheduled_ = true;
}
