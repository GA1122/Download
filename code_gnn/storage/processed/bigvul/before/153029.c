bool PDFiumEngineExports::GetPDFDocInfo(const void* pdf_buffer,
                                        int buffer_size,
                                        int* page_count,
                                        double* max_page_width) {
  FPDF_DOCUMENT doc = FPDF_LoadMemDocument(pdf_buffer, buffer_size, nullptr);
  if (!doc)
    return false;
  int page_count_local = FPDF_GetPageCount(doc);
  if (page_count) {
    *page_count = page_count_local;
  }
  if (max_page_width) {
    *max_page_width = 0;
    for (int page_number = 0; page_number < page_count_local; page_number++) {
      double page_width = 0;
      double page_height = 0;
      FPDF_GetPageSizeByIndex(doc, page_number, &page_width, &page_height);
      if (page_width > *max_page_width) {
        *max_page_width = page_width;
      }
    }
  }
  FPDF_CloseDocument(doc);
  return true;
}
