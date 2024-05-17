WebstoreAPI::WebstoreAPI(content::BrowserContext* browser_context)
    : browser_context_(browser_context),
      install_observer_(
          new ScopedObserver<InstallTracker, InstallObserver>(this)) {
  install_observer_->Add(
      InstallTrackerFactory::GetForBrowserContext(browser_context));
}
