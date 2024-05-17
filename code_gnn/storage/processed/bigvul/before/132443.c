UsbDeviceImpl::~UsbDeviceImpl() {
  libusb_unref_device(platform_device_);
}
