 void DeviceTokenFetcher::StopAutoRetry() {
  scheduler_->CancelDelayedWork();
  backend_.reset();
// void DeviceTokenFetcher::Reset() {
//   SetState(STATE_INACTIVE);
  }