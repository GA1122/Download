bool DefaultTabHandler::CanDuplicateContentsAt(int index) {
  return delegate_->AsBrowser()->CanDuplicateContentsAt(index);
}
