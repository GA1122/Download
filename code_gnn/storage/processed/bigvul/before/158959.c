bool PDFiumEngine::HandleDocumentLoad(const pp::URLLoader& loader) {
  password_tries_remaining_ = kMaxPasswordTries;
  process_when_pending_request_complete_ = true;
  auto loader_wrapper =
      std::make_unique<URLLoaderWrapperImpl>(GetPluginInstance(), loader);
  loader_wrapper->SetResponseHeaders(headers_);

  doc_loader_ = std::make_unique<DocumentLoader>(this);
  if (doc_loader_->Init(std::move(loader_wrapper), url_)) {
    doc_loader_->RequestData(0, 1);
    return true;
  }
  return false;
}
