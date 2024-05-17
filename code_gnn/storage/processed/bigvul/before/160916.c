DOMWindow* DOMWindow::opener() const {
  if (!GetFrame() || !GetFrame()->Client())
    return nullptr;

  Frame* opener = GetFrame()->Client()->Opener();
  return opener ? opener->DomWindow() : nullptr;
}
