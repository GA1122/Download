FPDF_BOOL PDFiumEngine::IsDataAvail(FX_FILEAVAIL* param,
                                    size_t offset,
                                    size_t size) {
  auto* file_avail = static_cast<FileAvail*>(param);
  return file_avail->engine->doc_loader_->IsDataAvailable(offset, size);
}
