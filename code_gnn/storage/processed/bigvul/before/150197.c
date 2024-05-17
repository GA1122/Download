  void set_profile_to_activate(Profile* profile) {
    profile_to_activate_ = profile;
    MaybeActivateProfile();
  }
