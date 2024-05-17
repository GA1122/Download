void Browser::OpenURLOffTheRecord(Profile* profile, const GURL& url) {
  Browser* browser = GetOrCreateTabbedBrowser(
      profile->GetOffTheRecordProfile());
  browser->AddSelectedTabWithURL(url, PageTransition::LINK);
  browser->window()->Show();
}
