void NavigationControllerImpl::GoBack() {
  GoToIndex(GetIndexForOffset(-1));
}
