std::string PDFiumEngine::GetMetadata(const std::string& key) {
  return GetDocumentMetadata(doc(), key);
}
