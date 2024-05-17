void CloudPolicyController::Retry() {
  scheduler_->CancelDelayedWork();
   DoWork();
 }
