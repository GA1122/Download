int PepperMediaDeviceManager::GetSessionID(PP_DeviceType_Dev type,
                                           const std::string& label) {
#if defined(ENABLE_WEBRTC)
  switch (type) {
    case PP_DEVICETYPE_DEV_AUDIOCAPTURE:
      return GetMediaStreamDispatcher()->audio_session_id(label, 0);
    case PP_DEVICETYPE_DEV_VIDEOCAPTURE:
      return GetMediaStreamDispatcher()->video_session_id(label, 0);
    default:
      NOTREACHED();
      return 0;
  }
#else
  return 0;
#endif
}
