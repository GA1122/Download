MetricsWebContentsObserver::TestingObserver::~TestingObserver() {
  if (observer_) {
    observer_->RemoveTestingObserver(this);
    observer_ = nullptr;
  }
}
