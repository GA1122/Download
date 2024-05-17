void ResourceMultiBufferDataProvider::StartLoading(
    std::unique_ptr<WebURLRequest> request) {
  active_loader_->LoadAsynchronously(*request, this);
}
