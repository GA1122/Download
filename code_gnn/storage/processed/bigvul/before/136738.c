void LocalDOMWindow::ClearEventQueue() {
  if (!event_queue_)
    return;
  event_queue_->Close();
  event_queue_.Clear();
}
