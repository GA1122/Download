TouchEventConverterEvdev::TouchEventConverterEvdev(
    int fd,
    base::FilePath path,
    int id,
    InputDeviceType type,
    const EventDeviceInfo& devinfo,
    DeviceEventDispatcherEvdev* dispatcher)
    : EventConverterEvdev(fd,
                          path,
                          id,
                          type,
                          devinfo.name(),
                          devinfo.vendor_id(),
                          devinfo.product_id()),
      dispatcher_(dispatcher),
      syn_dropped_(false),
      has_mt_(false),
      touch_points_(0),
      next_tracking_id_(0),
      current_slot_(0) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kExtraTouchNoiseFiltering)) {
    touch_noise_finder_.reset(new TouchNoiseFinder);
  }
}
