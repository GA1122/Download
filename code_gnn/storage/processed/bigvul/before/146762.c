EventListener* Document::GetWindowAttributeEventListener(
    const AtomicString& event_type) {
  LocalDOMWindow* dom_window = this->domWindow();
  if (!dom_window)
    return 0;
  return dom_window->GetAttributeEventListener(event_type);
}
