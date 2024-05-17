PrefService* InitializeLocalState(
    base::SequencedTaskRunner* local_state_task_runner,
    const base::CommandLine& parsed_command_line) {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::InitializeLocalState")

  PrefService* local_state = g_browser_process->local_state();
  DCHECK(local_state);

#if defined(OS_WIN)
  if (first_run::IsChromeFirstRun()) {
    base::string16 install_lang;
    if (GoogleUpdateSettings::GetLanguage(&install_lang)) {
      local_state->SetString(prefs::kApplicationLocale,
                             base::UTF16ToASCII(install_lang));
    }
  }
#endif   

  if (parsed_command_line.HasSwitch(switches::kParentProfile)) {
    base::FilePath local_state_path;
    PathService::Get(chrome::FILE_LOCAL_STATE, &local_state_path);
    bool local_state_file_exists = base::PathExists(local_state_path);
    if (!local_state_file_exists) {
      base::FilePath parent_profile =
          parsed_command_line.GetSwitchValuePath(switches::kParentProfile);
      scoped_refptr<PrefRegistrySimple> registry = new PrefRegistrySimple();
      scoped_ptr<PrefService> parent_local_state(
          chrome_prefs::CreateLocalState(
              parent_profile,
              local_state_task_runner,
              g_browser_process->policy_service(),
              registry,
              false));
      registry->RegisterStringPref(prefs::kApplicationLocale, std::string());
      local_state->SetString(
          prefs::kApplicationLocale,
          parent_local_state->GetString(prefs::kApplicationLocale));
    }
  }

#if defined(OS_CHROMEOS)
  if (parsed_command_line.HasSwitch(chromeos::switches::kLoginManager)) {
    std::string owner_locale = local_state->GetString(prefs::kOwnerLocale);
    if (!owner_locale.empty() &&
        local_state->GetString(prefs::kApplicationLocale) != owner_locale &&
        !local_state->IsManagedPreference(prefs::kApplicationLocale)) {
      local_state->SetString(prefs::kApplicationLocale, owner_locale);
    }
  }
#endif   

  return local_state;
}
