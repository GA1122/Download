void GetDevicesOnServiceThread(
    const std::vector<UsbDeviceFilter>& filters,
    const base::Callback<void(mojo::Array<DeviceInfoPtr>)>& callback,
    scoped_refptr<base::TaskRunner> callback_task_runner) {
  DCHECK(DeviceClient::Get());
  UsbService* usb_service = DeviceClient::Get()->GetUsbService();
  if (!usb_service) {
    mojo::Array<DeviceInfoPtr> no_devices(0);
    callback_task_runner->PostTask(
        FROM_HERE, base::Bind(callback, base::Passed(&no_devices)));
    return;
  }
  usb_service->GetDevices(base::Bind(&OnGetDevicesOnServiceThread, filters,
                                     callback, callback_task_runner));
}
