void* MapFontWithMetrics(FPDF_SYSFONTINFO* sysfontinfo,
                         int weight,
                         int italic,
                         int charset,
                         int pitch_family,
                         const char* face,
                         int* exact) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->MapFont)
    return nullptr;
  void* mapped_font = fontinfo_with_metrics->default_sysfontinfo->MapFont(
      fontinfo_with_metrics->default_sysfontinfo, weight, italic, charset,
      pitch_family, face, exact);
  if (mapped_font && g_engine_for_fontmapper)
    g_engine_for_fontmapper->FontSubstituted();
  return mapped_font;
}
