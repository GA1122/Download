bool TestNavigationManager::WaitForDesiredState() {
  if (current_state_ == desired_state_)
    return true;

  if (navigation_paused_)
    handle_->CallResumeForTesting();

  if (current_state_ < desired_state_) {
    DCHECK(!loop_runner_);
    loop_runner_ = new MessageLoopRunner();
    loop_runner_->Run();
    loop_runner_ = nullptr;
  }

  return current_state_ == desired_state_;
}
