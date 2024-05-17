bool ChromeContentBrowserClientExtensionsPart::ShouldSwapProcessesForRedirect(
    content::BrowserContext* browser_context,
    const GURL& current_url,
    const GURL& new_url) {
  return CrossesExtensionProcessBoundary(
      ExtensionRegistry::Get(browser_context)->enabled_extensions(),
      current_url, new_url, false);
}
