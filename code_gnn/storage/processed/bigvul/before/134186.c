const char* CodeFromNative(const base::NativeEvent& native_event) {
  const uint16 scan_code = GetScanCodeFromLParam(native_event.lParam);
  return CodeForWindowsScanCode(scan_code);
}
