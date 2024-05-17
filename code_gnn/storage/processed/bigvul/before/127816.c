int PepperMediaDeviceManager::OpenDevice(PP_DeviceType_Dev type,
                                         const std::string& device_id,
                                         const GURL& document_url,
                                         const OpenDeviceCallback& callback) {
  open_callbacks_[next_id_] = callback;
  int request_id = next_id_++;

#if defined(ENABLE_WEBRTC)
  GetMediaStreamDispatcher()->OpenDevice(
      request_id,
      AsWeakPtr(),
      device_id,
      PepperMediaDeviceManager::FromPepperDeviceType(type),
      document_url.GetOrigin());
#else
  base::MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&PepperMediaDeviceManager::OnDeviceOpenFailed,
                 AsWeakPtr(),
                 request_id));
#endif

  return request_id;
}
