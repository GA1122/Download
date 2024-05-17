void TestNavigationManager::OnNavigationStateChanged() {
  if (current_state_ >= desired_state_) {
    if (loop_runner_)
      loop_runner_->Quit();
    return;
  }

  if (navigation_paused_)
    handle_->CallResumeForTesting();
}
