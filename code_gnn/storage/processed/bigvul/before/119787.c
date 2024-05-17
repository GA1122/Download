void NavigationControllerImpl::GoToOffset(int offset) {
  if (!CanGoToOffset(offset))
    return;

  GoToIndex(GetIndexForOffset(offset));
}
