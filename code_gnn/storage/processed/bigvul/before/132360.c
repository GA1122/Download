void OnOpenDeviceOnServiceThread(
    mojo::InterfaceRequest<Device> device_request,
    const DeviceManager::OpenDeviceCallback& callback,
    scoped_refptr<base::TaskRunner> callback_task_runner,
    scoped_refptr<UsbDeviceHandle> device_handle) {
  if (!device_handle) {
    callback_task_runner->PostTask(FROM_HERE,
                                   base::Bind(&RunOpenDeviceCallback, callback,
                                              OPEN_DEVICE_ERROR_ACCESS_DENIED));
    return;
  }

  new DeviceImpl(device_handle, device_request.Pass());

  callback_task_runner->PostTask(
      FROM_HERE,
      base::Bind(&RunOpenDeviceCallback, callback, OPEN_DEVICE_ERROR_OK));
}
