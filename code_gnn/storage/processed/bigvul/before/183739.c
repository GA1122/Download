 void CloudPolicyController::StopAutoRetry() {
  scheduler_->CancelDelayedWork();
  backend_.reset();
// void CloudPolicyController::Reset() {
//   SetState(STATE_TOKEN_UNAVAILABLE);
  }