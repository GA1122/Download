int32_t PepperDeviceEnumerationHostHelper::InternalHandleResourceMessage(
    const IPC::Message& msg,
    HostMessageContext* context,
    bool* handled) {
  *handled = true;
  PPAPI_BEGIN_MESSAGE_MAP(PepperDeviceEnumerationHostHelper, msg)
    PPAPI_DISPATCH_HOST_RESOURCE_CALL_0(
        PpapiHostMsg_DeviceEnumeration_EnumerateDevices, OnEnumerateDevices)
    PPAPI_DISPATCH_HOST_RESOURCE_CALL(
        PpapiHostMsg_DeviceEnumeration_MonitorDeviceChange,
        OnMonitorDeviceChange)
    PPAPI_DISPATCH_HOST_RESOURCE_CALL_0(
        PpapiHostMsg_DeviceEnumeration_StopMonitoringDeviceChange,
        OnStopMonitoringDeviceChange)
  PPAPI_END_MESSAGE_MAP()

  *handled = false;
  return PP_ERROR_FAILED;
}
