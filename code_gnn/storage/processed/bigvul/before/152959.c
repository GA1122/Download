PDFEngine* PDFEngine::Create(PDFEngine::Client* client) {
  return new PDFiumEngine(client);
}
