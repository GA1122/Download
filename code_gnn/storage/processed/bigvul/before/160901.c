DOMWindow* DOMWindow::AnonymousIndexedGetter(uint32_t index) const {
  if (!GetFrame())
    return nullptr;

  Frame* child = GetFrame()->Tree().ScopedChild(index);
  return child ? child->DomWindow() : nullptr;
}
