gfx::Rect ExtensionInstalledBubble::GetAnchorRect() {
  if (type_ == OMNIBOX_KEYWORD) {
    LocationBarView* location_bar_view =
        BrowserView::GetBrowserViewForBrowser(browser_)->GetLocationBarView();
    return gfx::Rect(location_bar_view->GetLocationEntryOrigin(),
        gfx::Size(0, location_bar_view->location_entry_view()->height()));
  }
  return views::BubbleDelegateView::GetAnchorRect();
}
