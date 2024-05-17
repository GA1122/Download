int PepperMediaDeviceManager::EnumerateDevices(
    PP_DeviceType_Dev type,
    const GURL& document_url,
    const EnumerateDevicesCallback& callback) {
  enumerate_callbacks_[next_id_] = callback;
  int request_id = next_id_++;

#if defined(ENABLE_WEBRTC)
  GetMediaStreamDispatcher()->EnumerateDevices(
      request_id,
      AsWeakPtr(),
      PepperMediaDeviceManager::FromPepperDeviceType(type),
      document_url.GetOrigin());
#else
  base::MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&PepperMediaDeviceManager::OnDevicesEnumerated,
                 AsWeakPtr(),
                 request_id,
                 StreamDeviceInfoArray()));
#endif

  return request_id;
}
