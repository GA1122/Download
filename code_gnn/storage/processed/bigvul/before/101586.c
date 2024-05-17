bool Browser::CanGoBack() const {
  return GetSelectedTabContentsWrapper()->controller().CanGoBack();
}
