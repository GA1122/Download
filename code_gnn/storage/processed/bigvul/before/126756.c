BrowserView* BrowserView::GetBrowserViewForBrowser(const Browser* browser) {
  return static_cast<BrowserView*>(browser->window());
}
