void PDFiumEngine::FinishLoadingDocument() {
  DCHECK(doc_loader_->IsDocumentComplete() && doc_);

  LoadBody();

  bool need_update = false;
  for (size_t i = 0; i < pages_.size(); ++i) {
    if (pages_[i]->available())
      continue;

    pages_[i]->set_available(true);
    FPDFAvail_IsPageAvail(fpdf_availability_, i, &download_hints_);
    need_update = true;
    if (IsPageVisible(i))
      client_->Invalidate(GetPageScreenRect(i));
  }
  if (need_update)
    LoadPageInfo(true);

  if (called_do_document_action_)
    return;
  called_do_document_action_ = true;

  FORM_DoDocumentJSAction(form_);
  FORM_DoDocumentOpenAction(form_);
  if (most_visible_page_ != -1) {
    FPDF_PAGE new_page = pages_[most_visible_page_]->GetPage();
    FORM_DoPageAAction(new_page, form_, FPDFPAGE_AACTION_OPEN);
  }

  if (doc_) {
    DocumentFeatures document_features;
    document_features.page_count = pages_.size();
    document_features.has_attachments = (FPDFDoc_GetAttachmentCount(doc_) > 0);
    document_features.is_linearized =
        (FPDFAvail_IsLinearized(fpdf_availability_) == PDF_LINEARIZED);
    document_features.is_tagged = FPDFCatalog_IsTagged(doc_);
    document_features.form_type = static_cast<FormType>(FPDF_GetFormType(doc_));
    client_->DocumentLoadComplete(document_features);
  }
}
