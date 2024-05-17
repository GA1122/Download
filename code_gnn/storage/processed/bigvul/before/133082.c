gfx::Rect HWNDMessageHandler::GetClientAreaBounds() const {
  if (IsMinimized())
    return gfx::Rect();
  if (delegate_->WidgetSizeIsClientSize())
    return GetClientAreaBoundsInScreen();
  return GetWindowBoundsInScreen();
}
