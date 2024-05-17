int MockPrinter::CreateDocumentCookie() {
  return use_invalid_settings_ ? 0 : ++current_document_cookie_;
}
