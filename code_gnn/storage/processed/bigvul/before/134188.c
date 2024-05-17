int EventFlagsFromNative(const base::NativeEvent& native_event) {
  int flags = KeyStateFlagsFromNative(native_event);
  if (IsMouseEvent(native_event))
    flags |= MouseStateFlagsFromNative(native_event);

  return flags;
}
