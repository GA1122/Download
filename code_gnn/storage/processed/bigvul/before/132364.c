void DeviceManagerImpl::set_connection_error_handler(
    const mojo::Closure& error_handler) {
  binding_.set_connection_error_handler(error_handler);
}
