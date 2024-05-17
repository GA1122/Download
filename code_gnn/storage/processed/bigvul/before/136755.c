EventQueue* LocalDOMWindow::GetEventQueue() const {
  return event_queue_.Get();
}
