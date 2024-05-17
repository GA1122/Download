bool NavigationControllerImpl::IsInitialBlankNavigation() const {
  return IsInitialNavigation() && GetEntryCount() == 0;
}
