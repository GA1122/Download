PP_DeviceType_Dev PepperMediaDeviceManager::FromMediaStreamType(
    MediaStreamType type) {
  switch (type) {
    case MEDIA_NO_SERVICE:
      return PP_DEVICETYPE_DEV_INVALID;
    case MEDIA_DEVICE_AUDIO_CAPTURE:
      return PP_DEVICETYPE_DEV_AUDIOCAPTURE;
    case MEDIA_DEVICE_VIDEO_CAPTURE:
      return PP_DEVICETYPE_DEV_VIDEOCAPTURE;
    default:
      NOTREACHED();
      return PP_DEVICETYPE_DEV_INVALID;
  }
}
