unsigned long GetFontData(FPDF_SYSFONTINFO* sysfontinfo,
                          void* hFont,
                          unsigned int table,
                          unsigned char* buffer,
                          unsigned long buf_size) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->GetFontData)
    return 0;
  return fontinfo_with_metrics->default_sysfontinfo->GetFontData(
      fontinfo_with_metrics->default_sysfontinfo, hFont, table, buffer,
      buf_size);
}
