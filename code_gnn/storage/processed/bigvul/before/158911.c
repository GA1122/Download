void DeleteFont(FPDF_SYSFONTINFO* sysfontinfo, void* font_id) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->DeleteFont)
    return;
  fontinfo_with_metrics->default_sysfontinfo->DeleteFont(
      fontinfo_with_metrics->default_sysfontinfo, font_id);
}
