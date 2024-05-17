bool NavigationControllerImpl::CanGoBack() const {
  return CanGoToOffset(-1);
}
