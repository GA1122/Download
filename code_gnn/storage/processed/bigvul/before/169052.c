base::Time OfflinePageModelImpl::GetCurrentTime() const {
  return testing_clock_ ? testing_clock_->Now() : base::Time::Now();
}
