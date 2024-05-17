bool Browser::CanGoForward() const {
  return GetSelectedTabContentsWrapper()->controller().CanGoForward();
}
