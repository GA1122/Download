bool NavigationControllerImpl::CanGoForward() const {
  return CanGoToOffset(1);
}
