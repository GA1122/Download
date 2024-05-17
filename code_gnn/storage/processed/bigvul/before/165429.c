void StoragePartitionImpl::ResetURLLoaderFactories() {
  GetNetworkContext()->ResetURLLoaderFactories();
  url_loader_factory_for_browser_process_.reset();
  url_loader_factory_getter_->Initialize(this);
}
