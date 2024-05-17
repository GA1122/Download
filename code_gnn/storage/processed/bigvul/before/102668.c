DeviceTokenFetcher::~DeviceTokenFetcher() {
  scheduler_->CancelDelayedWork();
}
