void PDFiumEngine::FitContentsToPrintableAreaIfRequired(
    const FPDF_DOCUMENT& doc, const PP_PrintSettings_Dev& print_settings) {
  if (print_settings.print_scaling_option !=
          PP_PRINTSCALINGOPTION_SOURCE_SIZE) {
    int num_pages = FPDF_GetPageCount(doc);
    for (int i = 0; i < num_pages; ++i) {
      FPDF_PAGE page = FPDF_LoadPage(doc, i);
      TransformPDFPageForPrinting(page, print_settings);
      FPDF_ClosePage(page);
    }
  }
}
