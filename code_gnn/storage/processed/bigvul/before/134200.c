bool GetScrollOffsets(const base::NativeEvent& native_event,
                      float* x_offset,
                      float* y_offset,
                      float* x_offset_ordinal,
                      float* y_offset_ordinal,
                      int* finger_count) {
  if (native_event.message == WM_VSCROLL || native_event.message == WM_HSCROLL)
    return true;
  return false;
}
