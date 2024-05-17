Profile* GetFallbackStartupProfile() {
  ProfileManager* profile_manager = g_browser_process->profile_manager();

  auto* storage = &profile_manager->GetProfileAttributesStorage();
  for (Profile* profile : ProfileManager::GetLastOpenedProfiles()) {
    ProfileAttributesEntry* entry;
    bool has_entry = storage->GetProfileAttributesWithPath(profile->GetPath(),
                                                           &entry);
    if (!has_entry || !entry->IsSigninRequired())
      return profile;
  }

  Profile* guest_profile =
      profile_manager->GetProfile(ProfileManager::GetGuestProfilePath());
  Profile* system_profile =
      profile_manager->GetProfile(ProfileManager::GetSystemProfilePath());
  if (guest_profile && system_profile)
    return guest_profile;

  for (ProfileAttributesEntry* entry : storage->GetAllProfilesAttributes()) {
    if (!entry->IsSigninRequired()) {
      Profile* profile = profile_manager->GetProfile(entry->GetPath());
      if (profile)
        return profile;
    }
  }

  return nullptr;
}
