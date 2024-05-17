void ChromeBrowserMainPartsChromeos::PreProfileInit() {

  chromeos::BootTimesLoader::Get()->RecordChromeMainStats();

#if defined(TOOLKIT_USES_GTK)
  std::string gtkrc = l10n_util::GetStringUTF8(IDS_LOCALE_GTKRC);
  if (!gtkrc.empty())
    gtk_rc_parse_string(gtkrc.c_str());
#else
  NOTIMPLEMENTED();
#endif

  chromeos::OwnershipService::GetSharedInstance()->Prewarm();


  chromeos::ScreenLocker::InitClass();

  g_browser_process->profile_manager();

  if (parsed_command_line().HasSwitch(switches::kAllowFileAccess))
    net::URLRequest::AllowFileAccess();

  if (parsed_command_line().HasSwitch(switches::kLoginUser) &&
      !parsed_command_line().HasSwitch(switches::kLoginPassword)) {
    std::string username =
        parsed_command_line().GetSwitchValueASCII(switches::kLoginUser);
    VLOG(1) << "Relaunching browser for user: " << username;
    chromeos::UserManager::Get()->UserLoggedIn(username);

    logging::RedirectChromeLogging(parsed_command_line());

    g_browser_process->browser_policy_connector()->InitializeUserPolicy(
        username, false   );
  } else if (parsed_command_line().HasSwitch(switches::kLoginManager)) {
    chromeos::StatusAreaViewChromeos::
        SetScreenMode(chromeos::StatusAreaViewChromeos::LOGIN_MODE_WEBUI);
  }

  ChromeBrowserMainPartsLinux::PreProfileInit();
}
