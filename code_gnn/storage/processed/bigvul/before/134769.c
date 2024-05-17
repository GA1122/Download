EventConverterEvdevImpl::EventConverterEvdevImpl(
    int fd,
    base::FilePath path,
    int id,
    InputDeviceType type,
    const EventDeviceInfo& devinfo,
    CursorDelegateEvdev* cursor,
    DeviceEventDispatcherEvdev* dispatcher)
    : EventConverterEvdev(fd,
                          path,
                          id,
                          type,
                          devinfo.name(),
                          devinfo.vendor_id(),
                          devinfo.product_id()),
      has_keyboard_(devinfo.HasKeyboard()),
      has_touchpad_(devinfo.HasTouchpad()),
      has_caps_lock_led_(devinfo.HasLedEvent(LED_CAPSL)),
      x_offset_(0),
      y_offset_(0),
      cursor_(cursor),
      dispatcher_(dispatcher) {
}
