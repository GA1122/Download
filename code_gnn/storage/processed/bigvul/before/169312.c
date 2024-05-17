void TestNavigationManager::ResumeNavigation() {
  DCHECK(current_state_ == NavigationState::STARTED ||
         current_state_ == NavigationState::RESPONSE);
  DCHECK_EQ(current_state_, desired_state_);
  DCHECK(navigation_paused_);
  navigation_paused_ = false;
  handle_->CallResumeForTesting();
}
