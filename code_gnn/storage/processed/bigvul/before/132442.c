UsbDeviceImpl::UsbDeviceImpl(
    scoped_refptr<UsbContext> context,
    PlatformUsbDevice platform_device,
    uint16 vendor_id,
    uint16 product_id,
    scoped_refptr<base::SequencedTaskRunner> blocking_task_runner)
    : UsbDevice(vendor_id,
                product_id,
                base::string16(),
                base::string16(),
                base::string16()),
      platform_device_(platform_device),
      context_(context),
      task_runner_(base::ThreadTaskRunnerHandle::Get()),
      blocking_task_runner_(blocking_task_runner) {
  CHECK(platform_device) << "platform_device cannot be NULL";
  libusb_ref_device(platform_device);
  RefreshConfiguration();
}
