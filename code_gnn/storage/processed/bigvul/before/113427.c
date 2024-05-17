void InputMethodIBus::CreateContext() {
  DCHECK(!context_);
  DCHECK(GetBus());
  DCHECK(ibus_client_->IsConnected(GetBus()));
  DCHECK(!pending_create_ic_request_);

  pending_create_ic_request_ = new PendingCreateICRequestImpl(
      this, ibus_client_.get(), &pending_create_ic_request_);
  ibus_client_->CreateContext(GetBus(), pending_create_ic_request_);
}
