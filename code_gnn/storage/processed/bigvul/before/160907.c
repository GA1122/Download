bool DOMWindow::IsCurrentlyDisplayedInFrame() const {
  if (GetFrame())
    SECURITY_CHECK(GetFrame()->DomWindow() == this);
  return GetFrame() && GetFrame()->GetPage();
}
