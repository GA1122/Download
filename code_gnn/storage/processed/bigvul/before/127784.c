bool PepperDeviceEnumerationHostHelper::HandleResourceMessage(
    const IPC::Message& msg,
    HostMessageContext* context,
    int32_t* result) {
  bool return_value = false;
  *result = InternalHandleResourceMessage(msg, context, &return_value);
  return return_value;
}
