void BrowserInit::LaunchWithProfile::AddCrashedInfoBarIfNecessary(
    Browser* browser,
    TabContentsWrapper* tab) {
  if (!profile_->DidLastSessionExitCleanly() && !profile_->IsOffTheRecord()) {
    tab->infobar_tab_helper()->AddInfoBar(
        new SessionCrashedInfoBarDelegate(profile_, tab->infobar_tab_helper()));
  }
}
