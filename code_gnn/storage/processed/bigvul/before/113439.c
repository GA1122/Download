void InputMethodIBus::Init(bool focused) {
  IBusBus* bus = GetBus();

  g_signal_connect(bus, "connected",
                   G_CALLBACK(OnIBusConnectedThunk), this);
  g_signal_connect(bus, "disconnected",
                   G_CALLBACK(OnIBusDisconnectedThunk), this);

  if (ibus_client_->IsConnected(bus))
    CreateContext();

  InputMethodBase::Init(focused);
}
