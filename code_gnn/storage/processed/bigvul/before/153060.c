FPDF_BOOL PDFiumEngine::IsDataAvail(FX_FILEAVAIL* param,
                                    size_t offset, size_t size) {
  PDFiumEngine::FileAvail* file_avail =
      static_cast<PDFiumEngine::FileAvail*>(param);
  return file_avail->loader->IsDataAvailable(offset, size);
}
