DOMWindow* DOMWindow::parent() const {
  if (!GetFrame())
    return nullptr;

  Frame* parent = GetFrame()->Tree().Parent();
  return parent ? parent->DomWindow() : GetFrame()->DomWindow();
}
