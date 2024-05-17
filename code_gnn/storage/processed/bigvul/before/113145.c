  virtual void SetUp() {
    DCHECK(!download_manager_.get());

    mock_download_item_factory_ = (new MockDownloadItemFactory())->AsWeakPtr();
    mock_download_manager_delegate_.reset(
        new StrictMock<MockDownloadManagerDelegate>);
    EXPECT_CALL(*mock_download_manager_delegate_.get(), Shutdown())
        .WillOnce(Return());
    mock_download_file_manager_ = new StrictMock<MockDownloadFileManager>;
    EXPECT_CALL(*mock_download_file_manager_.get(),
                OnDownloadManagerShutdown(_));
    mock_browser_context_.reset(new StrictMock<MockBrowserContext>);
    EXPECT_CALL(*mock_browser_context_.get(), IsOffTheRecord())
        .WillRepeatedly(Return(false));

    download_manager_ = new DownloadManagerImpl(
        mock_download_file_manager_.get(),
        scoped_ptr<content::DownloadItemFactory>(
            mock_download_item_factory_.get()).Pass(), NULL);
    download_manager_->SetDelegate(mock_download_manager_delegate_.get());
    download_manager_->Init(mock_browser_context_.get());
  }
