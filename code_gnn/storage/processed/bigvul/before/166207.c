MediaStreamProvider* MediaStreamManager::GetDeviceManager(
    MediaStreamType stream_type) {
  if (IsVideoInputMediaType(stream_type))
    return video_capture_manager();
  else if (IsAudioInputMediaType(stream_type))
    return audio_input_device_manager();
  NOTREACHED();
  return nullptr;
}
