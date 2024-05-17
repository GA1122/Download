uint32_t PDFiumEngine::QuerySupportedPrintOutputFormats() {
  if (!HasPermission(PDFEngine::PERMISSION_PRINT_LOW_QUALITY))
    return 0;
  return PP_PRINTOUTPUTFORMAT_PDF;
}
