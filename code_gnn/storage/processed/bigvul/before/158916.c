void EnumFonts(FPDF_SYSFONTINFO* sysfontinfo, void* mapper) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->EnumFonts)
    return;
  fontinfo_with_metrics->default_sysfontinfo->EnumFonts(
      fontinfo_with_metrics->default_sysfontinfo, mapper);
}
