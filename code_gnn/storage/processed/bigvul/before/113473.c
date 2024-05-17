InputMethodIBus::PendingKeyEventImpl::~PendingKeyEventImpl() {
  if (input_method_)
    input_method_->FinishPendingKeyEvent(this);
}
