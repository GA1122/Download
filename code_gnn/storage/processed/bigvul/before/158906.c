std::unique_ptr<PDFEngine> PDFEngine::Create(PDFEngine::Client* client) {
  return std::make_unique<PDFiumEngine>(client);
}
