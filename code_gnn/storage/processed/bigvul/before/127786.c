int32_t PepperDeviceEnumerationHostHelper::OnEnumerateDevices(
    HostMessageContext* context) {
  if (enumerate_devices_context_.is_valid())
    return PP_ERROR_INPROGRESS;

  enumerate_.reset(new ScopedRequest(
      this,
      base::Bind(&PepperDeviceEnumerationHostHelper::OnEnumerateDevicesComplete,
                 base::Unretained(this))));
  if (!enumerate_->requested())
    return PP_ERROR_FAILED;

  enumerate_devices_context_ = context->MakeReplyMessageContext();
  return PP_OK_COMPLETIONPENDING;
}
