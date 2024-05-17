DOMWindow* DOMWindow::top() const {
  if (!GetFrame())
    return nullptr;

  return GetFrame()->Tree().Top().DomWindow();
}
