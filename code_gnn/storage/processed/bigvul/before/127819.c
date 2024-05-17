void PepperMediaDeviceManager::StopEnumerateDevicesDelayed(int request_id) {
#if defined(ENABLE_WEBRTC)
  if (render_frame())
    GetMediaStreamDispatcher()->StopEnumerateDevices(request_id, AsWeakPtr());
#endif
}
