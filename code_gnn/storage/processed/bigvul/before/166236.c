void MediaStreamManager::StartEnumeration(DeviceRequest* request,
                                          const std::string& label) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  media_devices_manager_->StartMonitoring();

  bool request_audio_input = request->audio_type() != MEDIA_NO_SERVICE;
  if (request_audio_input)
    request->SetState(request->audio_type(), MEDIA_REQUEST_STATE_REQUESTED);

  bool request_video_input = request->video_type() != MEDIA_NO_SERVICE;
  if (request_video_input)
    request->SetState(request->video_type(), MEDIA_REQUEST_STATE_REQUESTED);

  DCHECK(request_audio_input || request_video_input);
  MediaDevicesManager::BoolDeviceTypes devices_to_enumerate;
  devices_to_enumerate[MEDIA_DEVICE_TYPE_AUDIO_INPUT] = request_audio_input;
  devices_to_enumerate[MEDIA_DEVICE_TYPE_VIDEO_INPUT] = request_video_input;
  media_devices_manager_->EnumerateDevices(
      devices_to_enumerate,
      base::BindOnce(&MediaStreamManager::DevicesEnumerated,
                     base::Unretained(this), request_audio_input,
                     request_video_input, label));
}
