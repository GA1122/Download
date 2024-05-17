  void ClearExpectations(DownloadId id) {
    MockDownloadFile* file = download_file_factory_->GetExistingFile(id);
    Mock::VerifyAndClearExpectations(file);
    Mock::VerifyAndClearExpectations(download_manager_);
  }
