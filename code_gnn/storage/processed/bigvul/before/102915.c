bool DefaultTabHandler::CanCloseTab() const {
  return delegate_->AsBrowser()->CanCloseTab();
}
