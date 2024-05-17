EventQueue* Document::GetEventQueue() const {
  if (!dom_window_)
    return 0;
  return dom_window_->GetEventQueue();
}
