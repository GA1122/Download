void LocalDOMWindow::EnqueueWindowEvent(Event* event) {
  if (!event_queue_)
    return;
  event->SetTarget(this);
  event_queue_->EnqueueEvent(FROM_HERE, event);
}
