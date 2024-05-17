Browser* Browser::CreateForType(Type type, Profile* profile) {
  Browser* browser = new Browser(type, profile);
  browser->InitBrowserWindow();
  return browser;
}
