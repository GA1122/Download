bool BluetoothSocketListenFunction::PreRunValidation(std::string* error) {
  if (!BluetoothSocketAsyncApiFunction::PreRunValidation(error))
    return false;
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  EXTENSION_FUNCTION_PRERUN_VALIDATE(CreateParams());
  socket_event_dispatcher_ = GetSocketEventDispatcher(browser_context());
  return socket_event_dispatcher_ != nullptr;
}
