void OneClickSigninSyncStarter::ShowSyncSettingsPageOnSameTab() {
  std::string url = std::string(chrome::kChromeUISettingsURL) +
      chrome::kSyncSetupSubPage;
  chrome::NavigateParams params(
      browser_, GURL(url), content::PAGE_TRANSITION_AUTO_TOPLEVEL);
  params.disposition = CURRENT_TAB;
  params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  chrome::Navigate(&params);
}
