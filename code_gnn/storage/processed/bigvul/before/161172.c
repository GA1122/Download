bool MediaStreamManager::TranslateSourceIdToDeviceId(
    MediaStreamType stream_type,
    const std::string& salt,
    const url::Origin& security_origin,
    const std::string& source_id,
    std::string* device_id) const {
  DCHECK(stream_type == MEDIA_DEVICE_AUDIO_CAPTURE ||
         stream_type == MEDIA_DEVICE_VIDEO_CAPTURE);
  if (source_id.empty())
    return false;

  MediaDeviceInfoArray cached_devices =
      media_devices_manager_->GetCachedDeviceInfo(
          ConvertToMediaDeviceType(stream_type));
  return GetDeviceIDFromHMAC(salt, security_origin, source_id, cached_devices,
                             device_id);
}
