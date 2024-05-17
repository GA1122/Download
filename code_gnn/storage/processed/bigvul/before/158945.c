void* GetFont(FPDF_SYSFONTINFO* sysfontinfo, const char* face) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->GetFont)
    return nullptr;
  return fontinfo_with_metrics->default_sysfontinfo->GetFont(
      fontinfo_with_metrics->default_sysfontinfo, face);
}
