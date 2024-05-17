  void CreateTeamDrive(const std::string& name,
                       google_apis::TeamDriveCapabilities capabilities) {
    fake_drive_service_->AddTeamDrive(name, name);
    fake_drive_service_->SetTeamDriveCapabilities(name, capabilities);
  }
