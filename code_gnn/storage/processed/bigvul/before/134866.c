double GetUndocumentedDPIScale() {
  static double scale = -1.0;
  if (scale == -1.0) {
    scale = 1.0;
    if (!IsProcessDPIAwareWrapper()) {
      base::win::RegKey key(HKEY_CURRENT_USER,
                            L"Control Panel\\Desktop\\WindowMetrics",
                            KEY_QUERY_VALUE);
      if (key.Valid()) {
        DWORD value = 0;
        if (key.ReadValueDW(L"AppliedDPI", &value) == ERROR_SUCCESS) {
          scale = static_cast<double>(value) / kDefaultDPIX;
        }
      }
    }
  }
  return scale;
}
