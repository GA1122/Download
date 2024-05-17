  CardUnmaskDelegate* full_card_unmask_delegate() {
    DCHECK(autofill_manager_->full_card_request_);
    return static_cast<CardUnmaskDelegate*>(
        autofill_manager_->full_card_request_.get());
  }
