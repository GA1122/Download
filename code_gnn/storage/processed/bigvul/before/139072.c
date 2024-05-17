void AudioRendererHost::AuthorizationCompleted(
    int stream_id,
    const url::Origin& security_origin,
    base::TimeTicks auth_start_time,
    media::OutputDeviceStatus status,
    bool should_send_id,
    const media::AudioParameters& params,
    const std::string& raw_device_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  auto auth_data = authorizations_.find(stream_id);
  if (auth_data == authorizations_.end())
    return;   

  UMALogDeviceAuthorizationTime(auth_start_time);
  if (status == media::OUTPUT_DEVICE_STATUS_OK) {
    auth_data->second.first = true;
    auth_data->second.second = raw_device_id;
    if (should_send_id) {
      std::string hashed_id = MediaStreamManager::GetHMACForMediaDeviceID(
          salt_, security_origin, raw_device_id);
      Send(new AudioMsg_NotifyDeviceAuthorized(stream_id, status, params,
                                               hashed_id));
    } else {
      Send(new AudioMsg_NotifyDeviceAuthorized(stream_id, status, params,
                                               std::string()));
    }
  } else {
    authorizations_.erase(auth_data);
    Send(new AudioMsg_NotifyDeviceAuthorized(
        stream_id, status, media::AudioParameters::UnavailableDeviceParams(),
        std::string()));
  }
}
