  virtual void OnProfilePrepared(Profile* profile) {
    profile_prepared_ = true;
    chromeos::LoginUtils::DoBrowserLaunch(profile, NULL);
    if (!pending_requests_)
      delete this;
  }
