bool InitializeSDK() {
  SetUpV8();

  FPDF_LIBRARY_CONFIG config;
  config.version = 2;
  config.m_pUserFontPaths = nullptr;
  config.m_pIsolate = v8::Isolate::GetCurrent();
  config.m_v8EmbedderSlot = gin::kEmbedderPDFium;
  FPDF_InitLibraryWithConfig(&config);

#if defined(OS_LINUX)
  FPDF_SetSystemFontInfo(&g_font_info);
#else
  g_font_info =
      new FPDF_SYSFONTINFO_WITHMETRICS(FPDF_GetDefaultSystemFontInfo());
  g_font_info->Release = Release;
  g_font_info->EnumFonts = EnumFonts;
  g_font_info->MapFont = MapFontWithMetrics;
  g_font_info->GetFont = GetFont;
  g_font_info->GetFaceName = GetFaceName;
  g_font_info->GetFontCharset = GetFontCharset;
  g_font_info->GetFontData = GetFontData;
  g_font_info->DeleteFont = DeleteFont;
  FPDF_SetSystemFontInfo(g_font_info);
#endif

  FSDK_SetUnSpObjProcessHandler(&g_unsupported_info);

  return true;
}
