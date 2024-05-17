Browser* Browser::GetOrCreateTabbedBrowser(Profile* profile) {
  Browser* browser = GetTabbedBrowser(profile, false);
  if (!browser)
    browser = Browser::Create(profile);
  return browser;
}
