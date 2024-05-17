void MediaStreamManager::PostRequestToUI(
    const std::string& label,
    DeviceRequest* request,
    const MediaDeviceEnumeration& enumeration,
    const base::Optional<media::AudioParameters>& output_parameters) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(request->HasUIRequest());
  DCHECK(!output_parameters || output_parameters->IsValid());
  DVLOG(1) << "PostRequestToUI({label= " << label << "})";

  const MediaStreamType audio_type = request->audio_type();
  const MediaStreamType video_type = request->video_type();

  if (IsAudioInputMediaType(audio_type))
    request->SetState(audio_type, MEDIA_REQUEST_STATE_PENDING_APPROVAL);
  if (IsVideoMediaType(video_type))
    request->SetState(video_type, MEDIA_REQUEST_STATE_PENDING_APPROVAL);

  if (fake_ui_factory_ &&
      request->video_type() != MEDIA_DESKTOP_VIDEO_CAPTURE) {
    MediaStreamDevices devices = ConvertToMediaStreamDevices(
        request->audio_type(), enumeration[MEDIA_DEVICE_TYPE_AUDIO_INPUT]);
    MediaStreamDevices video_devices = ConvertToMediaStreamDevices(
        request->video_type(), enumeration[MEDIA_DEVICE_TYPE_VIDEO_INPUT]);
    devices.reserve(devices.size() + video_devices.size());
    devices.insert(devices.end(), video_devices.begin(), video_devices.end());

    std::unique_ptr<FakeMediaStreamUIProxy> fake_ui = fake_ui_factory_.Run();
    fake_ui->SetAvailableDevices(devices);

    request->ui_proxy = std::move(fake_ui);
  } else {
    request->ui_proxy = MediaStreamUIProxy::Create();
  }

  request->ui_proxy->RequestAccess(
      request->DetachUIRequest(),
      base::BindOnce(&MediaStreamManager::HandleAccessRequestResponse,
                     base::Unretained(this), label,
                     output_parameters.value_or(
                         media::AudioParameters::UnavailableDeviceParams())));
}
