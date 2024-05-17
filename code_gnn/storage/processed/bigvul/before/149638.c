TestObserver::~TestObserver() {
  predictor_->SetObserverForTesting(nullptr);
}
