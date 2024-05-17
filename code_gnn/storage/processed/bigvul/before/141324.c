LocalDOMWindow* Document::defaultView() const {
  return frame_ ? dom_window_ : nullptr;
}
