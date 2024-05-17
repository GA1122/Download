bool PDFiumEngine::IsProgressiveLoad() {
  return doc_loader_.is_partial_document();
}
