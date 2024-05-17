Browser* Browser::GetTabbedBrowser(Profile* profile, bool match_incognito) {
  return BrowserList::FindBrowserWithType(profile, TYPE_NORMAL,
                                          match_incognito);
}
