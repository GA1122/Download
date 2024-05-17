  bool Mount(Profile* profile) {
    if (profile != profile_)
      return false;

    if (!integration_service_)
      return false;

    integration_service_->SetEnabled(true);
    CreateDriveFsBootstrapListener();
    return true;
  }
