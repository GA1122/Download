void DownloadManagerImpl::SetDownloadItemFactoryForTesting(
    std::unique_ptr<DownloadItemFactory> item_factory) {
  item_factory_ = std::move(item_factory);
}
