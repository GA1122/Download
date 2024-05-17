Document* Document::ContextDocument() {
  if (context_document_)
    return context_document_;
  if (frame_)
    return this;
  return nullptr;
}
