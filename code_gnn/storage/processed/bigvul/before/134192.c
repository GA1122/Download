int GetChangedMouseButtonFlagsFromNative(
    const base::NativeEvent& native_event) {
  switch (GetNativeMouseKey(native_event)) {
    case MK_LBUTTON:
      return EF_LEFT_MOUSE_BUTTON;
    case MK_MBUTTON:
      return EF_MIDDLE_MOUSE_BUTTON;
    case MK_RBUTTON:
      return EF_RIGHT_MOUSE_BUTTON;
    default:
      break;
  }
  return 0;
 }
