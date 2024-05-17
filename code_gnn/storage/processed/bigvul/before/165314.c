void ChromeContentBrowserClient::RegisterRendererPreferenceWatcherForWorkers(
    content::BrowserContext* browser_context,
    content::mojom::RendererPreferenceWatcherPtr watcher) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  PrefWatcher::Get(profile)->RegisterWatcherForWorkers(std::move(watcher));
}
