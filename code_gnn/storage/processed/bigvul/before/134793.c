EventReaderLibevdevCros::~EventReaderLibevdevCros() {
  Stop();
  EvdevClose(&evdev_);
}
