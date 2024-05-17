bool HWNDMessageHandler::IsMaximized() const {
  return !!::IsZoomed(hwnd());
}
