bool BrowserView::IsRegularOrGuestSession() const {
  return profiles::IsRegularOrGuestSession(browser_.get());
}
