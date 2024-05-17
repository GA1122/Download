DocumentLoader* Document::Loader() const {
  if (!frame_)
    return 0;

  DocumentLoader* loader = frame_->Loader().GetDocumentLoader();
  if (!loader)
    return 0;

  if (frame_->GetDocument() != this)
    return 0;

  return loader;
}
