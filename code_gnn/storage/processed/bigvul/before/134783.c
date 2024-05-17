EventReaderLibevdevCros::EventReaderLibevdevCros(int fd,
                                                 const base::FilePath& path,
                                                 int id,
                                                 InputDeviceType type,
                                                 const EventDeviceInfo& devinfo,
                                                 scoped_ptr<Delegate> delegate)
    : EventConverterEvdev(fd,
                          path,
                          id,
                          type,
                          devinfo.name(),
                          devinfo.vendor_id(),
                          devinfo.product_id()),
      has_keyboard_(devinfo.HasKeyboard()),
      has_mouse_(devinfo.HasMouse()),
      has_touchpad_(devinfo.HasTouchpad()),
      has_caps_lock_led_(devinfo.HasLedEvent(LED_CAPSL)),
      delegate_(delegate.Pass()) {
  CHECK(!has_keyboard_ || type != INPUT_DEVICE_INTERNAL);

  memset(&evdev_, 0, sizeof(evdev_));
  evdev_.log = OnLogMessage;
  evdev_.log_udata = this;
  evdev_.syn_report = OnSynReport;
  evdev_.syn_report_udata = this;
  evdev_.fd = fd;

  memset(&evstate_, 0, sizeof(evstate_));
  evdev_.evstate = &evstate_;
  Event_Init(&evdev_);

  Event_Open(&evdev_);

  delegate_->OnLibEvdevCrosOpen(&evdev_, &evstate_);
}
