Profile* GetStartupProfile(const base::FilePath& user_data_dir,
                           const base::CommandLine& command_line) {
  ProfileManager* profile_manager = g_browser_process->profile_manager();

  base::FilePath profile_path =
      GetStartupProfilePath(user_data_dir, command_line);
  Profile* profile = profile_manager->GetProfile(profile_path);

  auto* storage = &profile_manager->GetProfileAttributesStorage();
  ProfileAttributesEntry* entry;
  bool has_entry = storage->GetProfileAttributesWithPath(profile_path, &entry);
  if (has_entry && (!entry->IsSigninRequired() || !profile)) {
    return profile;
  }

  return profile_manager->GetProfile(ProfileManager::GetSystemProfilePath()) ?
         profile_manager->GetProfile(ProfileManager::GetGuestProfilePath()) :
         nullptr;
}
