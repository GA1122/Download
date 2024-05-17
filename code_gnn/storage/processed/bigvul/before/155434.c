void ChromeContentBrowserClient::RegisterRendererPreferenceWatcher(
    content::BrowserContext* browser_context,
    blink::mojom::RendererPreferenceWatcherPtr watcher) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  PrefWatcher::Get(profile)->RegisterRendererPreferenceWatcher(
      std::move(watcher));
}
