bool BluetoothSocketAbstractConnectFunction::PreRunValidation(
    std::string* error) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!BluetoothSocketAsyncApiFunction::PreRunValidation(error))
    return false;

  params_ = bluetooth_socket::Connect::Params::Create(*args_);
  EXTENSION_FUNCTION_PRERUN_VALIDATE(params_.get());

  socket_event_dispatcher_ = GetSocketEventDispatcher(browser_context());
  return socket_event_dispatcher_ != nullptr;
}
