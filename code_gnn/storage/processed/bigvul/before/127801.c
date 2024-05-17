void PepperMediaDeviceManager::CancelOpenDevice(int request_id) {
  open_callbacks_.erase(request_id);

#if defined(ENABLE_WEBRTC)
  GetMediaStreamDispatcher()->CancelOpenDevice(request_id, AsWeakPtr());
#endif
}
