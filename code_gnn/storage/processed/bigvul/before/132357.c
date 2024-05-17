void DeviceManagerImpl::GetDevices(EnumerationOptionsPtr options,
                                   const GetDevicesCallback& callback) {
  auto filters = options->filters.To<std::vector<UsbDeviceFilter>>();
  auto get_devices_callback = base::Bind(&DeviceManagerImpl::OnGetDevices,
                                         weak_factory_.GetWeakPtr(), callback);
  service_task_runner_->PostTask(
      FROM_HERE,
      base::Bind(&GetDevicesOnServiceThread, filters, get_devices_callback,
                 base::ThreadTaskRunnerHandle::Get()));
}
