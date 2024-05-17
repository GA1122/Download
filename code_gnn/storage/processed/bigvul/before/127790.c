int32_t PepperDeviceEnumerationHostHelper::OnStopMonitoringDeviceChange(
    HostMessageContext*  ) {
  monitor_.reset(NULL);
  return PP_OK;
}
