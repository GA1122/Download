void ResourceMultiBufferDataProvider::SetDeferred(bool deferred) {
  if (active_loader_)
    active_loader_->SetDefersLoading(deferred);
}
