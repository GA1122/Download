EventQueue* Document::GetEventQueue() const {
  if (!dom_window_)
    return nullptr;
  return dom_window_->GetEventQueue();
}
