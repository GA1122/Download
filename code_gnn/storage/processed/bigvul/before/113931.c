  void ResetNotifier() {
    invalidation_notifier_->UpdateRegisteredIds(&mock_observer_, ObjectIdSet());
    message_loop_.RunAllPending();
     invalidation_notifier_.reset();
   }
