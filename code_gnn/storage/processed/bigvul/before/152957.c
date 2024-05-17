void PDFiumEngine::ContinueLoadingDocument(const std::string& password) {
  ScopedUnsupportedFeature scoped_unsupported_feature(this);

  bool needs_password = false;
  bool loaded = TryLoadingDoc(password, &needs_password);
  bool password_incorrect = !loaded && needs_password && !password.empty();
  if (password_incorrect && password_tries_remaining_ > 0) {
    GetPasswordAndLoad();
    return;
  }

  if (!doc_) {
    client_->DocumentLoadFailed();
    return;
  }

  if (FPDFDoc_GetPageMode(doc_) == PAGEMODE_USEOUTLINES)
    client_->DocumentHasUnsupportedFeature("Bookmarks");

  permissions_ = FPDF_GetDocPermissions(doc_);
  permissions_handler_revision_ = FPDF_GetSecurityHandlerRevision(doc_);

  if (!form_) {
    int form_status =
        FPDFAvail_IsFormAvail(fpdf_availability_, &download_hints_);
    bool doc_complete = doc_loader_.IsDocumentComplete();
    if (form_status == PDF_FORM_NOTAVAIL && !doc_complete)
      return;

    form_ = FPDFDOC_InitFormFillEnvironment(
        doc_, static_cast<FPDF_FORMFILLINFO*>(this));
#if defined(PDF_ENABLE_XFA)
    FPDF_LoadXFA(doc_);
#endif

    FPDF_SetFormFieldHighlightColor(form_, 0, kFormHighlightColor);
    FPDF_SetFormFieldHighlightAlpha(form_, kFormHighlightAlpha);
  }

  if (!doc_loader_.IsDocumentComplete()) {
    CheckPageAvailable(FPDFAvail_GetFirstPageNum(doc_), &pending_pages_);
  }

  LoadPageInfo(false);

  if (doc_loader_.IsDocumentComplete())
    FinishLoadingDocument();
}
