void DownloadManagerImpl::SetDownloadFileFactoryForTesting(
    std::unique_ptr<DownloadFileFactory> file_factory) {
  file_factory_ = std::move(file_factory);
}
