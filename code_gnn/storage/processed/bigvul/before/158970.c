void PDFiumEngine::LoadBody() {
  DCHECK(doc_);
  DCHECK(fpdf_availability_);
  if (doc_loader_->IsDocumentComplete()) {
    LoadForm();
  } else if (FPDFAvail_IsLinearized(fpdf_availability_) == PDF_LINEARIZED &&
             FPDF_GetPageCount(doc_) == 1) {
    LoadForm();
    if (form_status_ == PDF_FORM_NOTAVAIL)
      return;
  }
  LoadPages();
}
