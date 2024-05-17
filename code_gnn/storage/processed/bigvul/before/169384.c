bool TestNavigationManager::WaitForResponse() {
  desired_state_ = NavigationState::RESPONSE;
  return WaitForDesiredState();
}
