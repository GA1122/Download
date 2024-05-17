void PDFiumEngine::LoadForm() {
  if (form_)
    return;
  DCHECK(doc_);
  form_status_ = FPDFAvail_IsFormAvail(fpdf_availability_, &download_hints_);
  if (form_status_ != PDF_FORM_NOTAVAIL || doc_loader_->IsDocumentComplete()) {
    form_ = FPDFDOC_InitFormFillEnvironment(
        doc_, static_cast<FPDF_FORMFILLINFO*>(this));
#if defined(PDF_ENABLE_XFA)
    FPDF_LoadXFA(doc_);
#endif

    FPDF_SetFormFieldHighlightColor(form_, FPDF_FORMFIELD_UNKNOWN,
                                    kFormHighlightColor);
    FPDF_SetFormFieldHighlightAlpha(form_, kFormHighlightAlpha);
  }
}   
