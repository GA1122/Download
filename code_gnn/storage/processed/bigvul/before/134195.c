LPARAM GetLParamFromScanCode(uint16 scan_code) {
  LPARAM l_param = static_cast<LPARAM>(scan_code & 0x00FF) << 16;
  if ((scan_code & 0xE000) == 0xE000)
    l_param |= (1 << 24);
  return l_param;
}
