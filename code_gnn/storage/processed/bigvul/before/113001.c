  virtual void SetUp() {
    download_manager_ = new TestDownloadManager();
    request_handle_.reset(new MockDownloadRequestHandle(download_manager_));
    download_file_factory_ = new MockDownloadFileFactory;
    download_file_manager_ = new DownloadFileManager(download_file_factory_);
  }
