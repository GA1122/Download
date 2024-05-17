FrameFetchContext::GetPreviewsResourceLoadingHints() const {
  if (IsDetached())
    return nullptr;
  DocumentLoader* document_loader = MasterDocumentLoader();
  if (!document_loader)
    return nullptr;
  return document_loader->GetPreviewsResourceLoadingHints();
}
