MediaDeviceType ConvertToMediaDeviceType(MediaStreamType stream_type) {
  switch (stream_type) {
    case MEDIA_DEVICE_AUDIO_CAPTURE:
      return MEDIA_DEVICE_TYPE_AUDIO_INPUT;
    case MEDIA_DEVICE_VIDEO_CAPTURE:
      return MEDIA_DEVICE_TYPE_VIDEO_INPUT;
    default:
      NOTREACHED();
  }

  return NUM_MEDIA_DEVICE_TYPES;
}
