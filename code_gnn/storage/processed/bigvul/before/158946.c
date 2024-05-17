int GetFontCharset(FPDF_SYSFONTINFO* sysfontinfo, void* hFont) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->GetFontCharset)
    return 0;
  return fontinfo_with_metrics->default_sysfontinfo->GetFontCharset(
      fontinfo_with_metrics->default_sysfontinfo, hFont);
}
