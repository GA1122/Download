void PDFiumEngine::OnDocumentCanceled() {
  if (visible_pages_.empty())
    client_->DocumentLoadFailed();
  else
    OnDocumentComplete();
}
