bool IsMouseEvent(const base::NativeEvent& native_event) {
  return IsClientMouseEvent(native_event) ||
         IsNonClientMouseEvent(native_event);
}
