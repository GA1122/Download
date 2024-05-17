uint16 GetScanCodeFromLParam(LPARAM l_param) {
  uint16 scan_code = ((l_param >> 16) & 0x00FF);
  if (l_param & (1 << 24))
    scan_code |= 0xE000;
  return scan_code;
}
