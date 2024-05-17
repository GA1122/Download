Profile* CreatePrimaryProfile(const content::MainFunctionParams& parameters,
                              const base::FilePath& user_data_dir,
                              const base::CommandLine& parsed_command_line) {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::CreateProfile")
  TRACK_SCOPED_REGION(
      "Startup", "ChromeBrowserMainParts::CreatePrimaryProfile");

  base::Time start = base::Time::Now();
  if (profiles::IsMultipleProfilesEnabled() &&
      parsed_command_line.HasSwitch(switches::kProfileDirectory)) {
    profiles::SetLastUsedProfile(
        parsed_command_line.GetSwitchValueASCII(switches::kProfileDirectory));
    ListPrefUpdate update(g_browser_process->local_state(),
                          prefs::kProfilesLastActive);
    base::ListValue* profile_list = update.Get();
    profile_list->Clear();
  }

  Profile* profile = NULL;
#if defined(OS_CHROMEOS) || defined(OS_ANDROID)
  DCHECK_EQ(user_data_dir.value(),
            g_browser_process->profile_manager()->user_data_dir().value());
  profile = ProfileManager::GetActiveUserProfile();
#else
  base::FilePath profile_path =
      GetStartupProfilePath(user_data_dir, parsed_command_line);

  if (!switches::IsNewAvatarMenu() &&
      profile_path == ProfileManager::GetGuestProfilePath()) {
    profile_path = g_browser_process->profile_manager()->GetProfileInfoCache().
        GetPathOfProfileAtIndex(0);
  }
  profile = g_browser_process->profile_manager()->GetProfile(
      profile_path);

  if (switches::IsNewProfileManagement() &&
      profile &&
      !profile->IsGuestSession()) {
    ProfileInfoCache& cache =
        g_browser_process->profile_manager()->GetProfileInfoCache();
    size_t profile_index = cache.GetIndexOfProfileWithPath(profile_path);

    if (profile_index != std::string::npos &&
        cache.ProfileIsSigninRequiredAtIndex(profile_index)) {
      profile = g_browser_process->profile_manager()->GetProfile(
          ProfileManager::GetGuestProfilePath());
    }
  }
#endif   
  if (profile) {
    UMA_HISTOGRAM_LONG_TIMES(
        "Startup.CreateFirstProfile", base::Time::Now() - start);
    return profile;
  }

#if !defined(OS_WIN)
  CHECK(profile) << "Cannot get default profile.";
#endif   

  return NULL;
}
