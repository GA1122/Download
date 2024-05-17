 void CastConfigDelegateChromeos::LaunchCastOptions() {
  chrome::NavigateParams params(
      ProfileManager::GetActiveUserProfile(),
      FindCastExtension()->GetResourceURL("options.html"),
      ui::PAGE_TRANSITION_LINK);
  params.disposition = NEW_FOREGROUND_TAB;
  params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  chrome::Navigate(&params);
}
