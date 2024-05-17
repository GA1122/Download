Browser* ExtensionFunctionDispatcher::GetCurrentBrowser(
    RenderViewHost* render_view_host, bool include_incognito) {
  Browser* browser = delegate_->GetBrowser();

  if (browser)
    return browser;

  Profile* profile = render_view_host->process()->profile();
  browser = BrowserList::FindTabbedBrowser(profile, include_incognito);

  return browser;
}
