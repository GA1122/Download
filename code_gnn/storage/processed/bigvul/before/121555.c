  void SetUpOnUIThread() {
    fake_drive_service_.reset(new google_apis::FakeDriveService);
    fake_drive_service_->LoadResourceListForWapi(
        "chromeos/gdata/root_feed.json");
    fake_drive_service_->LoadAccountMetadataForWapi(
        "chromeos/gdata/account_metadata.json");
    fake_drive_service_->LoadAppListForDriveApi("chromeos/drive/applist.json");

    fake_drive_file_system_.reset(
        new test_util::FakeDriveFileSystem(fake_drive_service_.get()));
    fake_drive_file_system_->Initialize();
  }
