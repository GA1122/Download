  void AddLaunched(Profile* profile) {
    launched_profiles_.insert(profile);
    if (chrome::FindBrowserWithProfile(profile)) {
      opened_profiles_.insert(profile);
    }
  }
