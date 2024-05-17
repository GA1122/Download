Browser* GetOrCreateBrowser(Profile* profile) {
  Browser* browser = BrowserList::FindTabbedBrowser(profile, false);
  return browser ? browser : Browser::Create(profile);
}
