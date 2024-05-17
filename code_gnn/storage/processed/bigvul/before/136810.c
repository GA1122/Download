bool LocalDOMWindow::isSecureContext() const {
  if (!GetFrame())
    return false;

  return document()->IsSecureContext();
}
