StoragePartitionImpl::GetURLLoaderFactoryForBrowserProcess() {
  if (!shared_url_loader_factory_for_browser_process_) {
    shared_url_loader_factory_for_browser_process_ =
        new URLLoaderFactoryForBrowserProcess(this);
  }
  return shared_url_loader_factory_for_browser_process_;
}
