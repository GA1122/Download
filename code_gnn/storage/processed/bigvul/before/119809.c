void NavigationControllerImpl::ReloadOriginalRequestURL(bool check_for_repost) {
  ReloadInternal(check_for_repost, RELOAD_ORIGINAL_REQUEST_URL);
}
