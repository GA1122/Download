int DefaultTabHandler::GetDragActions() const {
  return delegate_->AsBrowser()->GetDragActions();
}
