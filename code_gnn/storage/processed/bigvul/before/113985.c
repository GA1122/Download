  Browser* GetAppropriateBrowser() {
    return browser::FindOrCreateTabbedBrowser(
        ProfileManager::GetDefaultProfileOrOffTheRecord());
  }
