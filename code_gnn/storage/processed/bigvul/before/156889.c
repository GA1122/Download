DocumentInit& DocumentInit::WithDocumentLoader(DocumentLoader* loader) {
  DCHECK(!document_loader_);
  DCHECK(!imports_controller_);
  document_loader_ = loader;
  if (document_loader_)
    parent_document_ = ParentDocument(document_loader_);
  return *this;
}
