bool IsNoopEvent(const base::NativeEvent& event) {
  return event.message == WM_USER + 310;
}
