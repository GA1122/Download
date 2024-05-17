uint32_t PDFiumEngine::QuerySupportedPrintOutputFormats() {
  if (HasPermission(PERMISSION_PRINT_HIGH_QUALITY))
    return PP_PRINTOUTPUTFORMAT_PDF | PP_PRINTOUTPUTFORMAT_RASTER;
  if (HasPermission(PERMISSION_PRINT_LOW_QUALITY))
    return PP_PRINTOUTPUTFORMAT_RASTER;
  return 0;
}
