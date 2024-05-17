void MediaStreamManager::PostRequestToUI(
    const std::string& label,
    const MediaDeviceEnumeration& enumeration,
    const base::Optional<media::AudioParameters>& output_parameters) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(!output_parameters || output_parameters->IsValid());
  DVLOG(1) << "PostRequestToUI({label= " << label << "})";

  DeviceRequest* request = FindRequest(label);
  if (!request)
    return;
  DCHECK(request->HasUIRequest());

  const MediaStreamType audio_type = request->audio_type();
  const MediaStreamType video_type = request->video_type();

  if (IsAudioInputMediaType(audio_type))
    request->SetState(audio_type, MEDIA_REQUEST_STATE_PENDING_APPROVAL);
  if (IsVideoInputMediaType(video_type))
    request->SetState(video_type, MEDIA_REQUEST_STATE_PENDING_APPROVAL);

  if (fake_ui_factory_) {
    MediaStreamDevices devices;
    if (request->video_type() == MEDIA_DISPLAY_VIDEO_CAPTURE) {
      devices.push_back(MediaStreamDeviceFromFakeDeviceConfig());
    } else if (request->video_type() == MEDIA_GUM_DESKTOP_VIDEO_CAPTURE) {
      MediaStreamDevice device;
      if (request->request_type() == MEDIA_DEVICE_UPDATE) {
        DesktopMediaID media_id(DesktopMediaID::TYPE_WINDOW,
                                DesktopMediaID::kNullId);
        device = MediaStreamDevice(MEDIA_GUM_DESKTOP_VIDEO_CAPTURE,
                                   media_id.ToString(), label);
      } else {
        DesktopMediaID media_id(DesktopMediaID::TYPE_SCREEN,
                                DesktopMediaID::kNullId);
        device = MediaStreamDevice(MEDIA_GUM_DESKTOP_VIDEO_CAPTURE,
                                   media_id.ToString(), label);
      }
      devices.push_back(device);
    } else {
      MediaStreamDevices audio_devices = ConvertToMediaStreamDevices(
          request->audio_type(), enumeration[MEDIA_DEVICE_TYPE_AUDIO_INPUT]);
      MediaStreamDevices video_devices = ConvertToMediaStreamDevices(
          request->video_type(), enumeration[MEDIA_DEVICE_TYPE_VIDEO_INPUT]);
      devices.reserve(audio_devices.size() + video_devices.size());
      devices.insert(devices.end(), audio_devices.begin(), audio_devices.end());
      devices.insert(devices.end(), video_devices.begin(), video_devices.end());
    }

    std::unique_ptr<FakeMediaStreamUIProxy> fake_ui = fake_ui_factory_.Run();
    fake_ui->SetAvailableDevices(devices);

    request->ui_proxy = std::move(fake_ui);
  } else if (!request->ui_proxy) {
    request->ui_proxy = MediaStreamUIProxy::Create();
  }

  request->ui_proxy->RequestAccess(
      request->DetachUIRequest(),
      base::BindOnce(&MediaStreamManager::HandleAccessRequestResponse,
                     base::Unretained(this), label,
                     output_parameters.value_or(
                         media::AudioParameters::UnavailableDeviceParams())));
}
