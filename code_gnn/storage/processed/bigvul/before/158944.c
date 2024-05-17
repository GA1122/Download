pp::Buffer_Dev PDFiumEngine::GetFlattenedPrintData(FPDF_DOCUMENT doc) {
  pp::Buffer_Dev buffer;
  ScopedSubstFont scoped_subst_font(this);
  int page_count = FPDF_GetPageCount(doc);
  for (int i = 0; i < page_count; ++i) {
    FPDF_PAGE page = FPDF_LoadPage(doc, i);
    DCHECK(page);
    int flatten_ret = FPDFPage_Flatten(page, FLAT_PRINT);
    FPDF_ClosePage(page);
    if (flatten_ret == FLATTEN_FAIL)
      return buffer;
  }

  PDFiumMemBufferFileWrite output_file_write;
  if (FPDF_SaveAsCopy(doc, &output_file_write, 0)) {
    size_t size = output_file_write.size();
    buffer = pp::Buffer_Dev(client_->GetPluginInstance(), size);
    if (!buffer.is_null())
      memcpy(buffer.data(), output_file_write.buffer().c_str(), size);
  }
  return buffer;
}
