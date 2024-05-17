MediaStreamType ConvertToMediaStreamType(MediaDeviceType type) {
  switch (type) {
    case MEDIA_DEVICE_TYPE_AUDIO_INPUT:
      return MEDIA_DEVICE_AUDIO_CAPTURE;
    case MEDIA_DEVICE_TYPE_VIDEO_INPUT:
      return MEDIA_DEVICE_VIDEO_CAPTURE;
    default:
      NOTREACHED();
  }

  return MEDIA_NO_SERVICE;
}
