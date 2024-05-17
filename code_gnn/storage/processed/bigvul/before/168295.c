std::string BrowserView::GetWindowName() const {
  return chrome::GetWindowName(browser_.get());
}
