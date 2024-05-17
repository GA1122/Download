void FrameLoader::DetachProvisionalDocumentLoader(DocumentLoader* loader) {
  DCHECK_EQ(loader, provisional_document_loader_);
  DetachDocumentLoader(provisional_document_loader_);
  DidFinishNavigation();
}
