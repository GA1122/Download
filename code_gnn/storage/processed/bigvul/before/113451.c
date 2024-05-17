void InputMethodIBus::OnIBusConnected(IBusBus* bus) {
  DCHECK_EQ(GetBus(), bus);
  DCHECK(ibus_client_->IsConnected(bus));

  DestroyContext();
  CreateContext();
}
