void InterstitialPageImpl::CancelForNavigation() {
  Disable();
  if (new_navigation_)
    TakeActionOnResourceDispatcher(RESUME);
  else
    TakeActionOnResourceDispatcher(CANCEL);
}