Browser* Browser::GetTabbedBrowser(Profile* profile, bool match_incognito) {
  return BrowserList::FindTabbedBrowser(profile, match_incognito);
}
