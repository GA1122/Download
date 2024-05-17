void AudioOutputAuthorizationHandler::TranslateDeviceID(
    AuthorizationCompletedCallback cb,
    const std::string& device_id,
    const url::Origin& security_origin,
    const MediaDeviceEnumeration& enumeration) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(!media::AudioDeviceDescription::IsDefaultDevice(device_id));
  for (const MediaDeviceInfo& device_info :
       enumeration[MEDIA_DEVICE_TYPE_AUDIO_OUTPUT]) {
    if (content::DoesMediaDeviceIDMatchHMAC(salt_, security_origin, device_id,
                                            device_info.device_id)) {
      GetDeviceParameters(std::move(cb), device_info.device_id);
      return;
    }
  }
  cb.Run(media::OUTPUT_DEVICE_STATUS_ERROR_NOT_FOUND, false,
         media::AudioParameters::UnavailableDeviceParams(), std::string());
}
