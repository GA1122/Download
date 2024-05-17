bool IsMouseWheelEvent(const base::NativeEvent& native_event) {
  return native_event.message == WM_MOUSEWHEEL ||
         native_event.message == WM_MOUSEHWHEEL;
}
