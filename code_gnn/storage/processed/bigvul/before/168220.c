Browser* GetOrCreateBrowser(Profile* profile, bool user_gesture) {
  Browser* browser = chrome::FindTabbedBrowser(profile, false);
  return browser ? browser
                 : new Browser(Browser::CreateParams(profile, user_gesture));
}
