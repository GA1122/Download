void PDFiumEngine::OnPartialDocumentLoaded() {
  file_access_.m_FileLen = doc_loader_.document_size();
  if (!fpdf_availability_) {
    fpdf_availability_ = FPDFAvail_Create(&file_availability_, &file_access_);
    DCHECK(fpdf_availability_);
  }

  if (!FPDFAvail_IsLinearized(fpdf_availability_)) {
    doc_loader_.RequestData(0, doc_loader_.document_size());
    return;
  }

  LoadDocument();
}
