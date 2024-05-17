void ShutdownSDK() {
  FPDF_DestroyLibrary();
#if !defined(OS_LINUX)
  delete g_font_info;
#endif
  TearDownV8();
}
