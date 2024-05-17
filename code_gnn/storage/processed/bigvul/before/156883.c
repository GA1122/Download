DocumentLoader* DocumentInit::MasterDocumentLoader() const {
  if (document_loader_)
    return document_loader_;
  if (imports_controller_) {
    return imports_controller_->Master()
        ->GetFrame()
        ->Loader()
        .GetDocumentLoader();
  }
  return nullptr;
}
