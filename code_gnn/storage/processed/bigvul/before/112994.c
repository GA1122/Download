  void CreateDownloadFile(scoped_ptr<DownloadCreateInfo> info) {
    download_file_manager_->CreateDownloadFile(
        info.Pass(), scoped_ptr<content::ByteStreamReader>(),
        download_manager_, true, net::BoundNetLog(),
        base::Bind(&DownloadFileManagerTest::OnDownloadFileCreated,
                   base::Unretained(this)));

    last_reason_ = content::DOWNLOAD_INTERRUPT_REASON_FILE_ACCESS_DENIED;
    ProcessAllPendingMessages();
    EXPECT_EQ(content::DOWNLOAD_INTERRUPT_REASON_NONE, last_reason_);
  }
