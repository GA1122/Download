int KeyStateFlagsFromNative(const base::NativeEvent& native_event) {
  int flags = 0;
  flags |= base::win::IsAltPressed() ? EF_ALT_DOWN : EF_NONE;
  flags |= base::win::IsShiftPressed() ? EF_SHIFT_DOWN : EF_NONE;
  flags |= base::win::IsCtrlPressed() ? EF_CONTROL_DOWN : EF_NONE;

  if (IsKeyEvent(native_event))
    flags |= (HIWORD(native_event.lParam) & KF_EXTENDED) ? EF_EXTENDED : 0;

  if (IsClientMouseEvent(native_event)) {
    int win_flags = GET_KEYSTATE_WPARAM(native_event.wParam);
    flags |= (win_flags & MK_SHIFT) ? EF_SHIFT_DOWN : 0;
    flags |= (win_flags & MK_CONTROL) ? EF_CONTROL_DOWN : 0;
  }

  return flags;
}
