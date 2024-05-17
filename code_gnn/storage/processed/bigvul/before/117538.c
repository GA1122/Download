void ChromeBrowserMainPartsChromeos::PostProfileInit() {

  if (parsed_command_line().HasSwitch(switches::kLoginUser) &&
      !parsed_command_line().HasSwitch(switches::kLoginPassword)) {
    g_browser_process->browser_policy_connector()->SetUserPolicyTokenService(
        profile()->GetTokenService());
  }

  if (!parameters().ui_task)
    OptionallyRunChromeOSLoginManager(parsed_command_line(), profile());

  ChromeBrowserMainPartsLinux::PostProfileInit();
}
