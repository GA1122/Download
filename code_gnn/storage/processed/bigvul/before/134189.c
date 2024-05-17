gfx::Point EventLocationFromNative(const base::NativeEvent& native_event) {
  if (IsClientMouseEvent(native_event) && !IsMouseWheelEvent(native_event))
    return gfx::Point(native_event.lParam);
  DCHECK(IsNonClientMouseEvent(native_event) ||
         IsMouseWheelEvent(native_event) || IsScrollEvent(native_event));
  POINT native_point;
  if (IsScrollEvent(native_event)) {
    ::GetCursorPos(&native_point);
  } else {
    native_point.x = GET_X_LPARAM(native_event.lParam);
    native_point.y = GET_Y_LPARAM(native_event.lParam);
  }
  ScreenToClient(native_event.hwnd, &native_point);
  gfx::Point location(native_point);
  location = gfx::win::ScreenToDIPPoint(location);
  return location;
}
