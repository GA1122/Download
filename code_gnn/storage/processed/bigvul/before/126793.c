std::string BrowserView::GetWindowName() const {
  return chrome::GetWindowPlacementKey(browser_.get());
}
