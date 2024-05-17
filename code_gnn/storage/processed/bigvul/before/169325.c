bool TestNavigationManager::ShouldMonitorNavigation(NavigationHandle* handle) {
  if (handle_ || handle->GetURL() != url_)
    return false;
  if (current_state_ != NavigationState::INITIAL)
    return false;
  return true;
}
