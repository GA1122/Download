static const OSExchangeData::CustomFormat& GetRendererTaintCustomType() {
  CR_DEFINE_STATIC_LOCAL(
      ui::OSExchangeData::CustomFormat,
      format,
      (ui::Clipboard::GetFormatType("chromium/x-renderer-taint")));
  return format;
}
