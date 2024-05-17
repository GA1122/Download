bool DefaultTabHandler::CanRestoreTab() {
  return delegate_->AsBrowser()->CanRestoreTab();
}
