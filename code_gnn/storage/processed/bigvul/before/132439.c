 void UsbDeviceImpl::OpenOnBlockingThreadWithFd(dbus::FileDescriptor fd,
                                                const OpenCallback& callback) {
   fd.CheckValidity();
  if (!fd.is_valid()) {
    USB_LOG(EVENT) << "Did not get valid device handle from permission broker.";
    task_runner_->PostTask(FROM_HERE, base::Bind(callback, nullptr));
    return;
  }

  PlatformUsbDeviceHandle handle;
  const int rv = libusb_open_fd(platform_device_, fd.TakeValue(), &handle);
  if (LIBUSB_SUCCESS == rv) {
    task_runner_->PostTask(
        FROM_HERE, base::Bind(&UsbDeviceImpl::Opened, this, handle, callback));
  } else {
    USB_LOG(EVENT) << "Failed to open device: "
                   << ConvertPlatformUsbErrorToString(rv);
    task_runner_->PostTask(FROM_HERE, base::Bind(callback, nullptr));
  }
}
