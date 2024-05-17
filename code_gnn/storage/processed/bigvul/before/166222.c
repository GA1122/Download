void MediaStreamManager::ReadOutputParamsAndPostRequestToUI(
    const std::string& label,
    DeviceRequest* request,
    const MediaDeviceEnumeration& enumeration) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (request->audio_type() == MEDIA_GUM_TAB_AUDIO_CAPTURE) {
    audio_system_->GetOutputStreamParameters(
        media::AudioDeviceDescription::kDefaultDeviceId,
        base::BindOnce(&MediaStreamManager::PostRequestToUI,
                       base::Unretained(this), label, enumeration));
  } else {
    PostRequestToUI(label, enumeration,
                    base::Optional<media::AudioParameters>());
  }
}
