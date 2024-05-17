bool IsChromeBrowserPresent() {
  return IsProductInstalled(USER_LEVEL_INSTALLATION, kBrowserAppGuid) ||
      IsProductInstalled(SYSTEM_LEVEL_INSTALLATION, kBrowserAppGuid);
}
