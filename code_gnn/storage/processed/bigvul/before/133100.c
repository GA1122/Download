bool HWNDMessageHandler::IsVisible() const {
  return !!::IsWindowVisible(hwnd());
}
