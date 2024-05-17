base::Time OfflinePageModelTaskified::GetCurrentTime() {
  CHECK(clock_);
  return clock_->Now();
}
