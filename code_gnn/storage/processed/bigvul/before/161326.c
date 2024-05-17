  explicit DevtoolsClearCacheObserver(
      content::BrowsingDataRemover* remover,
      std::unique_ptr<NetworkHandler::ClearBrowserCacheCallback> callback)
      : remover_(remover), callback_(std::move(callback)) {
    remover_->AddObserver(this);
  }
