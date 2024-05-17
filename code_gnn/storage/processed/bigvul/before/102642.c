CloudPolicyController::~CloudPolicyController() {
  data_store_->RemoveObserver(this);
  scheduler_->CancelDelayedWork();
}
