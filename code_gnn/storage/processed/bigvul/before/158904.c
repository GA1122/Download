void PDFiumEngine::ContinueLoadingDocument(const std::string& password) {
  ScopedUnsupportedFeature scoped_unsupported_feature(this);
  ScopedSubstFont scoped_subst_font(this);

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

  LoadBody();

  if (doc_loader_->IsDocumentComplete())
    FinishLoadingDocument();
}
