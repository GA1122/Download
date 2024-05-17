void OpenDeviceOnServiceThread(
    const std::string& guid,
    mojo::InterfaceRequest<Device> device_request,
    const DeviceManager::OpenDeviceCallback& callback,
    scoped_refptr<base::TaskRunner> callback_task_runner) {
  DCHECK(DeviceClient::Get());
  UsbService* usb_service = DeviceClient::Get()->GetUsbService();
  if (!usb_service) {
    callback_task_runner->PostTask(FROM_HERE,
                                   base::Bind(&RunOpenDeviceCallback, callback,
                                              OPEN_DEVICE_ERROR_NOT_FOUND));
    return;
  }
  scoped_refptr<UsbDevice> device = usb_service->GetDevice(guid);
  if (!device) {
    callback_task_runner->PostTask(FROM_HERE,
                                   base::Bind(&RunOpenDeviceCallback, callback,
                                              OPEN_DEVICE_ERROR_NOT_FOUND));
    return;
  }
  device->Open(base::Bind(&OnOpenDeviceOnServiceThread,
                          base::Passed(&device_request), callback,
                          callback_task_runner));
}
