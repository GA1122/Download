bool PDFiumEngine::HandleDocumentLoad(const pp::URLLoader& loader) {
  password_tries_remaining_ = kMaxPasswordTries;
  return doc_loader_.Init(loader, url_, headers_);
}
