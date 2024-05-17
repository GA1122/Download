void TestNavigationManager::OnWillProcessResponse() {
  current_state_ = NavigationState::RESPONSE;
  navigation_paused_ = true;
  OnNavigationStateChanged();
}
