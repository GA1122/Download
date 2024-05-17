void DownloadManagerImpl::SetDownloadFileFactoryForTesting(
    std::unique_ptr<download::DownloadFileFactory> file_factory) {
  in_progress_manager_->set_file_factory(std::move(file_factory));
}
