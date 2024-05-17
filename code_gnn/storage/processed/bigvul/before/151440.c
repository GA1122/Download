inline DocumentLoader* FrameFetchContext::MasterDocumentLoader() const {
  DCHECK(!IsDetached());

  if (document_loader_)
    return document_loader_.Get();

  return FrameOfImportsController()->Loader().GetDocumentLoader();
}
