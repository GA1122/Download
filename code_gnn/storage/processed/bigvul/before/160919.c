DOMWindow* DOMWindow::self() const {
  if (!GetFrame())
    return nullptr;

  return GetFrame()->DomWindow();
}
