void InlineLoginHandlerImpl::HandleLoginError(const std::string& error_msg,
                                              const base::string16& email) {
  SyncStarterCallback(OneClickSigninSyncStarter::SYNC_SETUP_FAILURE);
  Browser* browser = GetDesktopBrowser();
  Profile* profile = Profile::FromWebUI(web_ui());

  if (IsSystemProfile(profile))
    profile = g_browser_process->profile_manager()->GetProfileByPath(
        UserManager::GetSigninProfilePath());
  CloseModalSigninIfNeeded(this);
  if (!error_msg.empty()) {
    LoginUIServiceFactory::GetForProfile(profile)->DisplayLoginResult(
        browser, base::UTF8ToUTF16(error_msg), email);
  }
}
