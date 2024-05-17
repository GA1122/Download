void BluetoothSocketListenUsingRfcommFunction::CreateService(
    scoped_refptr<device::BluetoothAdapter> adapter,
    const device::BluetoothUUID& uuid,
    std::unique_ptr<std::string> name,
    const device::BluetoothAdapter::CreateServiceCallback& callback,
    const device::BluetoothAdapter::CreateServiceErrorCallback&
        error_callback) {
  device::BluetoothAdapter::ServiceOptions service_options;
  service_options.name = std::move(name);

  ListenOptions* options = params_->options.get();
  if (options) {
    if (options->channel.get())
      service_options.channel.reset(new int(*(options->channel)));
  }

  adapter->CreateRfcommService(uuid, service_options, callback, error_callback);
}
