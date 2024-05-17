  bool HasBeenLaunched(const Profile* profile) const {
    return launched_profiles_.find(profile) != launched_profiles_.end();
  }
