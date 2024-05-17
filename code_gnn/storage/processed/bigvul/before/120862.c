void OneClickSigninSyncStarter::ConfigureSync() {
  ProfileSyncService* profile_sync_service = GetProfileSyncService();
  LoginUIService* login_ui = LoginUIServiceFactory::GetForProfile(profile_);
  if (login_ui->current_login_ui()) {
    login_ui->current_login_ui()->FocusUI();
  } else {
    EnsureBrowser();
    if (profile_sync_service) {
      if (force_same_tab_navigation_) {
        ShowSyncSettingsPageOnSameTab();
      } else {
        chrome::ShowSettingsSubPage(browser_, chrome::kSyncSetupSubPage);
      }
    } else {
      FinishProfileSyncServiceSetup();
      chrome::ShowSettings(browser_);
    }
  }
}
