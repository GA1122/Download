bool HWNDMessageHandler::IsMinimized() const {
  return !!::IsIconic(hwnd());
}
