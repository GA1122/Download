void PDFiumEngine::OnDocumentComplete() {
  if (doc_) {
    return FinishLoadingDocument();
  }
  file_access_.m_FileLen = doc_loader_->GetDocumentSize();
  if (!fpdf_availability_) {
    fpdf_availability_ = FPDFAvail_Create(&file_availability_, &file_access_);
    DCHECK(fpdf_availability_);
  }
  LoadDocument();
}
