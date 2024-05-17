unsigned long GetFaceName(FPDF_SYSFONTINFO* sysfontinfo,
                          void* hFont,
                          char* buffer,
                          unsigned long buffer_size) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->GetFaceName)
    return 0;
  return fontinfo_with_metrics->default_sysfontinfo->GetFaceName(
      fontinfo_with_metrics->default_sysfontinfo, hFont, buffer, buffer_size);
}
