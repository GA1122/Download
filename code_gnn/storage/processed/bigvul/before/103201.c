void Browser::ShowPageInfo(Profile* profile,
                           const GURL& url,
                           const NavigationEntry::SSLStatus& ssl,
                           bool show_history) {
  window()->ShowPageInfo(profile, url, ssl, show_history);
}
