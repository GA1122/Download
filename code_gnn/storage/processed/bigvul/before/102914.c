 bool DefaultTabHandler::CanBookmarkAllTabs() const {
  return delegate_->AsBrowser()->CanBookmarkAllTabs();
}
