ui::AXMode ChromeContentBrowserClient::GetAXModeForBrowserContext(
    content::BrowserContext* browser_context) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  return AccessibilityLabelsServiceFactory::GetForProfile(profile)->GetAXMode();
}
