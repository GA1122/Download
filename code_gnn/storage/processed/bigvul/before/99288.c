void ResourceMessageFilter::OnDocumentDetached(unsigned long long document_id) {
  WorkerService::GetInstance()->DocumentDetached(this, document_id);
}
