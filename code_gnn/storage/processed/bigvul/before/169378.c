void TestNavigationManager::WaitForNavigationFinished() {
  desired_state_ = NavigationState::FINISHED;
  WaitForDesiredState();
}
