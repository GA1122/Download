void DeviceManagerImpl::OnGetDevices(const GetDevicesCallback& callback,
                                     mojo::Array<DeviceInfoPtr> devices) {
  mojo::Array<DeviceInfoPtr> allowed_devices(0);
  for (size_t i = 0; i < devices.size(); ++i) {
    if (delegate_->IsDeviceAllowed(*devices[i]))
      allowed_devices.push_back(devices[i].Pass());
  }
  callback.Run(allowed_devices.Pass());
}
