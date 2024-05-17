int MouseStateFlagsFromNative(const base::NativeEvent& native_event) {
  int win_flags = GetNativeMouseKey(native_event);

  if (IsClientMouseEvent(native_event))
    win_flags |= GET_KEYSTATE_WPARAM(native_event.wParam);

  int flags = 0;
  flags |= (win_flags & MK_LBUTTON) ? EF_LEFT_MOUSE_BUTTON : 0;
  flags |= (win_flags & MK_MBUTTON) ? EF_MIDDLE_MOUSE_BUTTON : 0;
  flags |= (win_flags & MK_RBUTTON) ? EF_RIGHT_MOUSE_BUTTON : 0;
  flags |= IsNonClientMouseEvent(native_event) ? EF_IS_NON_CLIENT : 0;
  return flags;
}
