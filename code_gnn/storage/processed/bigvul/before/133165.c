void HWNDMessageHandler::SetCapture() {
  DCHECK(!HasCapture());
  ::SetCapture(hwnd());
}
