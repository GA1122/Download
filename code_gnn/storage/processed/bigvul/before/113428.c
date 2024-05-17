void InputMethodIBus::DestroyContext() {
  if (pending_create_ic_request_) {
    DCHECK(!context_);
    pending_create_ic_request_->Abandon();
    pending_create_ic_request_ = NULL;
  } else if (context_) {
    ibus_client_->DestroyProxy(context_);
    DCHECK(!context_);
  }
}
