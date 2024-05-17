void DeviceManagerImpl::OpenDevice(
    const mojo::String& guid,
    mojo::InterfaceRequest<Device> device_request,
    const OpenDeviceCallback& callback) {
  service_task_runner_->PostTask(
      FROM_HERE, base::Bind(&OpenDeviceOnServiceThread, guid,
                            base::Passed(&device_request), callback,
                            base::ThreadTaskRunnerHandle::Get()));
}
