Document* FetchManager::Loader::GetDocument() const {
  if (execution_context_->IsDocument()) {
    return ToDocument(execution_context_);
  }
  return nullptr;
}
