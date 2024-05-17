void DownloadManagerImpl::SetDownloadItemFactoryForTesting(
    std::unique_ptr<download::DownloadItemFactory> item_factory) {
  item_factory_ = std::move(item_factory);
}
