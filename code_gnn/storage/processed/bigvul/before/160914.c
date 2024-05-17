bool DOMWindow::closed() const {
  return window_is_closing_ || !GetFrame() || !GetFrame()->GetPage();
}
