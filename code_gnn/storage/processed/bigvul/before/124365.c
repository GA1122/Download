void BrowserEventRouter::OnBrowserAdded(Browser* browser) {
  RegisterForBrowserNotifications(browser);
}
