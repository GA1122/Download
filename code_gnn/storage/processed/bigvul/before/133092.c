bool HWNDMessageHandler::HasCapture() const {
  return ::GetCapture() == hwnd();
}
