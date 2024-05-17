 BOOL IsProcessDPIAwareWrapper() {
   typedef BOOL(WINAPI *IsProcessDPIAwarePtr)(VOID);
   IsProcessDPIAwarePtr is_process_dpi_aware_func =
      reinterpret_cast<IsProcessDPIAwarePtr>(
          GetProcAddress(GetModuleHandleA("user32.dll"), "IsProcessDPIAware"));
  if (is_process_dpi_aware_func)
    return is_process_dpi_aware_func();
  return FALSE;
}
