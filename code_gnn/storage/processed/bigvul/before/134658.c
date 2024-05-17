OSExchangeDataProviderAuraX11::~OSExchangeDataProviderAuraX11() {
  if (own_window_) {
    base::MessagePumpX11::Current()->RemoveDispatcherForWindow(x_window_);
    XDestroyWindow(x_display_, x_window_);
  }
}
