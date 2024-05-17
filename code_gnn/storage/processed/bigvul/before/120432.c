  virtual ~WindowedPersonalDataManagerObserver() {
    if (infobar_service_) {
      while (infobar_service_->infobar_count() > 0) {
        infobar_service_->RemoveInfoBar(infobar_service_->infobar_at(0));
      }
    }
  }
