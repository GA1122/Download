void Browser::ShowPageInfo(content::BrowserContext* browser_context,
                           const GURL& url,
                           const NavigationEntry::SSLStatus& ssl,
                           bool show_history) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  window()->ShowPageInfo(profile, url, ssl, show_history);
}
