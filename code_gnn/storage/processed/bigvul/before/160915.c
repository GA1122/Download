unsigned DOMWindow::length() const {
  return GetFrame() ? GetFrame()->Tree().ScopedChildCount() : 0;
}
