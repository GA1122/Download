bool IsScrollEvent(const base::NativeEvent& native_event) {
  return native_event.message == WM_VSCROLL ||
         native_event.message == WM_HSCROLL;
}
