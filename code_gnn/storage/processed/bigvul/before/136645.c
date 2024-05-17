void FrameLoader::ClientDroppedNavigation() {
  if (!provisional_document_loader_ || provisional_document_loader_->DidStart())
    return;

  DetachProvisionalDocumentLoader(provisional_document_loader_);
}