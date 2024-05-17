void PepperDeviceEnumerationHostHelper::OnEnumerateDevicesComplete(
    int  ,
    const std::vector<ppapi::DeviceRefData>& devices) {
  DCHECK(enumerate_devices_context_.is_valid());

  enumerate_.reset(NULL);

  enumerate_devices_context_.params.set_result(PP_OK);
  resource_host_->host()->SendReply(
      enumerate_devices_context_,
      PpapiPluginMsg_DeviceEnumeration_EnumerateDevicesReply(devices));
  enumerate_devices_context_ = ppapi::host::ReplyMessageContext();
}
