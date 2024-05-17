browser::NavigateParams Browser::GetSingletonTabNavigateParams(
    const GURL& url) {
  browser::NavigateParams params(this, url, PageTransition::AUTO_BOOKMARK);
  params.disposition = SINGLETON_TAB;
  params.window_action = browser::NavigateParams::SHOW_WINDOW;
  return params;
}
