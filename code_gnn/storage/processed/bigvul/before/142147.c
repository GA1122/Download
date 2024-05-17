  drive::DriveIntegrationService* CreateDriveIntegrationService(
      Profile* profile) {
    if (!CreateRootDirectory(profile))
      return nullptr;

    EXPECT_FALSE(profile_);
    profile_ = profile;

    EXPECT_FALSE(fake_drive_service_);
    fake_drive_service_ = new drive::FakeDriveService;

    EXPECT_FALSE(integration_service_);
    integration_service_ = new drive::DriveIntegrationService(
        profile, nullptr, fake_drive_service_, std::string(),
        root_path().Append("v1"), nullptr, CreateDriveFsBootstrapListener());

    return integration_service_;
  }
