bool DocumentInit::ShouldSetURL() const {
  DocumentLoader* loader = MasterDocumentLoader();
  return (loader && loader->GetFrame()->Tree().Parent()) || !url_.IsEmpty();
}
