  virtual ~WindowedPersonalDataManagerObserver() {
    if (!infobar_service_)
      return;

    InfoBarDelegate* infobar = NULL;
    if (infobar_service_->GetInfoBarCount() > 0 &&
        (infobar = infobar_service_->GetInfoBarDelegateAt(0))) {
      infobar_service_->RemoveInfoBar(infobar);
    }
  }
