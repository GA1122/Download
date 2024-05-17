void PepperDeviceEnumerationHostHelper::OnNotifyDeviceChange(
    uint32_t callback_id,
    int  ,
    const std::vector<ppapi::DeviceRefData>& devices) {
  resource_host_->host()->SendUnsolicitedReply(
      resource_host_->pp_resource(),
      PpapiPluginMsg_DeviceEnumeration_NotifyDeviceChange(callback_id,
                                                          devices));
}
