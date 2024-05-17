void PDFiumEngine::OnNewDataReceived() {
  client_->DocumentLoadProgress(doc_loader_->count_of_bytes_received(),
                                doc_loader_->GetDocumentSize());
}
