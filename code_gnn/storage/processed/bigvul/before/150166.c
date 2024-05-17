  void ActivateProfile() {
    if (profile_to_activate_) {
      Browser* browser = chrome::FindBrowserWithProfile(profile_to_activate_);
      if (browser)
        browser->window()->Activate();
      profile_to_activate_ = nullptr;
    }
    activated_profile_ = true;
  }
